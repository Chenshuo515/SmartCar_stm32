#include "Ultrasound.h"
#include "tim.h"


// 本文件静态全局变量
static volatile uint32_t echo_start = 0, echo_end = 0;
static volatile uint16_t echo_start_tick = 0, echo_end_tick = 0;
static volatile uint8_t echo_flag = 0;  // 0:空闲 1:上升沿捕获 2:下降沿捕获完成
static volatile US_StateTypeDef us_state = US_IDLE;
static volatile float distance = 0.0f;
static volatile uint16_t trig_tick = 0;  // 触发时的10ms系统时标



#define TIM2_US_PERIOD      10000U    // TIM2配置：10ms = 10000us
#define MEASURE_TIMEOUT_TICK 6U       // 6*10ms=60ms超时
#define TRIG_PULSE_US       20U       // HC-SR04要求至少10us高电平触发,这里20保证容错

// 简单微秒延时（中断内短延时，不阻塞主循环）
static void us_delay_us(uint32_t us)
{
    uint32_t clk = SystemCoreClock / 1000000U;
    while(us--)
    {
        for(uint32_t i = 0; i < clk; i++);
    }
}

// 10ms定时器中断调用：超声波非阻塞状态机
void Ultrasonic_Timer_ISR_Handler(void)
{
    switch(us_state)
    {
        case US_IDLE:
            break;

        case US_TRIGGER:
            // 输出10us触发脉冲
            HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
            us_delay_us(TRIG_PULSE_US);
            HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);

            trig_tick = Flag_10ms_Cnt;    // 记录本次触发时的系统时标
            echo_flag = 0;             // 清空上次回波标志
            us_state = US_WAIT_ECHO;
            break;

        case US_WAIT_ECHO:
            // 30ms超时保护
            if((Flag_10ms_Cnt - trig_tick) >= MEASURE_TIMEOUT_TICK)
            {
                us_state = US_TRIGGER;//新一轮测距
                break;
            }
            // 完整捕获上升+下降沿，进入计算阶段
            if(echo_flag == 2)
            {
                us_state = US_CALCULATE;
            }
            break;

        case US_CALCULATE:
        {
            uint16_t tick_diff = echo_end_tick - echo_start_tick;
            uint32_t total_us;

            if(echo_end >= echo_start)
            {
                // 未跨TIM2溢出周期
                total_us = tick_diff * TIM2_US_PERIOD + (echo_end - echo_start);
            }
            else
            {
                // 跨溢出周期，补全差值
                total_us = (tick_diff - 1) * TIM2_US_PERIOD + (TIM2_US_PERIOD - echo_start + echo_end);
            }

            // 声速340m/s，往返除以2 这里单位为cm
            distance = (total_us * 0.034f) / 2.0f;
            us_state = US_TRIGGER;//新一轮测距
            break;
        }
    }
}

// 外部调用：手动启动一次测距（非阻塞）
void Ultrasonic_Start_Measure(void)
{
    if(us_state == US_IDLE)
    {
        us_state = US_TRIGGER;
    }
}

// 外部调用：读取最新测量距离
// 返回-1.0f代表超时/测量失败
float Ultrasonic_Get_Distance(void)
{
    return distance;
}

// ECHO外部中断回调：捕获上升沿/下降沿
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == ECHO_Pin)
    {
        if(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_SET)
        {
            // ECHO上升沿，记录TIM2计数值+当前10ms时标
            echo_start = __HAL_TIM_GET_COUNTER(&htim2);
            echo_start_tick = Flag_10ms_Cnt;
            echo_flag = 1;
        }
        else
        {
            // ECHO下降沿，记录结束计数值+时标
            echo_end = __HAL_TIM_GET_COUNTER(&htim2);
            echo_end_tick = Flag_10ms_Cnt;
            echo_flag = 2;
        }
    }
}