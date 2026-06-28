#include "delay.h"
#include "tim.h"  // 引入 CubeMX 生成的 TIM 头文件，获取 htim9 定义

static uint32_t fac_us = 0;
volatile uint32_t nowtime = 0;  // 时基计数器，100μs 递增一次

// 初始化延迟函数（同时确保 TIM9 启动，若已在 main 初始化可省略重复启动）
void delay_init(void)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    fac_us = HAL_RCC_GetHCLKFreq() / 1000000;
}

// 延时 nus（原有逻辑，保持不变）
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;

    ticks = nus * fac_us;
    told = SysTick->VAL;

    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += reload - tnow + told;

            told = tnow;
            if (tcnt >= ticks)
                break;
        }
    }
}

// 延时 nms（原有逻辑，保持不变）
void delay_ms(uint16_t nms)
{
    for (uint16_t i = 0; i < nms; i++)
    {
        delay_us(1000);
    }
}

// // 【新增】HAL 定时器周期中断回调，关联到 TIM9 更新时基
// 移植主函数中
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
//     if (htim == &htim9) {
//         nowtime++;  // 每 100μs 中断一次，计数器递增
//     }
// }

// 【新增】获取当前时基计数值（供姿态解算等逻辑使用）
uint32_t get_nowtime(void)
{
    return nowtime;
}