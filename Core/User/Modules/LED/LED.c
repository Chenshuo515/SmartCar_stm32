#include "LED.h"
#include "main.h"
#include "stm32f1xx_hal_gpio.h"

// 模式标记：1=推挽输出  0=输入高阻
static uint8_t g_R_Pin_Mode = 0;
static uint8_t g_L_Pin_Mode = 0;

//==================== 右组 LED 底层函数 ====================
void RQ_LED_ON(void)
{
    if(g_R_Pin_Mode != 1)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = R_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(R_LED_GPIO_Port, &GPIO_InitStruct);
        g_R_Pin_Mode = 1;
    }
    HAL_GPIO_WritePin(R_LED_GPIO_Port, R_LED_Pin, GPIO_PIN_RESET);
}

void RQ_LED_OFF(void)
{
    if(g_R_Pin_Mode != 0)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = R_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(R_LED_GPIO_Port, &GPIO_InitStruct);
        g_R_Pin_Mode = 0;
    }
}

void RH_LED_ON(void)
{
    if(g_R_Pin_Mode != 1)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = R_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(R_LED_GPIO_Port, &GPIO_InitStruct);
        g_R_Pin_Mode = 1;
    }
    HAL_GPIO_WritePin(R_LED_GPIO_Port, R_LED_Pin, GPIO_PIN_SET);
}

void RH_LED_OFF(void)
{
    if(g_R_Pin_Mode != 0)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = R_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(R_LED_GPIO_Port, &GPIO_InitStruct);
        g_R_Pin_Mode = 0;
    }
}

void ALLR_LED_ON(void)
{
    if(g_R_Pin_Mode != 1)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = R_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(R_LED_GPIO_Port, &GPIO_InitStruct);
        g_R_Pin_Mode = 1;
    }

    if(HAL_GPIO_ReadPin(R_LED_GPIO_Port, R_LED_Pin) == GPIO_PIN_SET)
    {
        HAL_GPIO_WritePin(R_LED_GPIO_Port, R_LED_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(R_LED_GPIO_Port, R_LED_Pin, GPIO_PIN_SET);
    }
}

void ALLR_LED_OFF(void)
{
    if(g_R_Pin_Mode != 0)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = R_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(R_LED_GPIO_Port, &GPIO_InitStruct);
        g_R_Pin_Mode = 0;
    }
}

//==================== 左组 LED 底层函数 ====================
void LQ_LED_ON(void)
{
    if(g_L_Pin_Mode != 1)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = L_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(L_LED_GPIO_Port, &GPIO_InitStruct);
        g_L_Pin_Mode = 1;
    }
    HAL_GPIO_WritePin(L_LED_GPIO_Port, L_LED_Pin, GPIO_PIN_RESET);
}

void LQ_LED_OFF(void)
{
    if(g_L_Pin_Mode != 0)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = L_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(L_LED_GPIO_Port, &GPIO_InitStruct);
        g_L_Pin_Mode = 0;
    }
}

void LH_LED_ON(void)
{
    if(g_L_Pin_Mode != 1)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = L_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(L_LED_GPIO_Port, &GPIO_InitStruct);
        g_L_Pin_Mode = 1;
    }
    HAL_GPIO_WritePin(L_LED_GPIO_Port, L_LED_Pin, GPIO_PIN_SET);
}

void LH_LED_OFF(void)
{
    if(g_L_Pin_Mode != 0)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = L_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(L_LED_GPIO_Port, &GPIO_InitStruct);
        g_L_Pin_Mode = 0;
    }
}

void ALLL_LED_ON(void)
{
    if(g_L_Pin_Mode != 1)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = L_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(L_LED_GPIO_Port, &GPIO_InitStruct);
        g_L_Pin_Mode = 1;
    }

    if(HAL_GPIO_ReadPin(L_LED_GPIO_Port, L_LED_Pin) == GPIO_PIN_SET)
    {
        HAL_GPIO_WritePin(L_LED_GPIO_Port, L_LED_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(L_LED_GPIO_Port, L_LED_Pin, GPIO_PIN_SET);
    }
}

void ALLL_LED_OFF(void)
{
    if(g_L_Pin_Mode != 0)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = L_LED_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(L_LED_GPIO_Port, &GPIO_InitStruct);
        g_L_Pin_Mode = 0;
    }
}

//==================== 呼吸使能控制 ====================
static void LED_Rear_Breath_Set(LED_Object_t *led, uint8_t en)
{
    led->rear_breath_en = en;
    led->pwm_cnt  = 0;
    led->div_cnt  = 0;
    led->breath_dir = 1;

    if(en == 0)
    {
        led->rear_duty = 0;
        RH_LED_OFF();
        LH_LED_OFF();
    }
}

//==================== 呼吸PWM状态机（修复：亮灭都渐变） ====================
#define LUT_SIZE        128

static const uint8_t breath_lut[LUT_SIZE] = {
        0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,
        3,3,3,4,4,5,5,6,6,7,8,8,9,10,11,12,
        13,14,15,16,17,18,19,20,21,22,23,25,26,27,29,30,
        32,33,35,36,38,40,41,43,45,47,49,50,52,54,56,58,
        60,60,58,56,54,52,50,49,47,45,43,41,40,38,36,35,
        33,32,30,29,27,26,25,23,22,21,20,19,18,17,16,15,
        14,13,12,11,10,9,8,8,7,6,6,5,5,4,4,3,
        3,3,2,2,2,2,1,1,1,1,1,0,0,0,0,0,0
};

static void LED_Group_Breath_Scan(LED_Object_t *led)
{
    if(!led->rear_breath_en) return;

    led->pwm_cnt++;
    led->div_cnt++;

    // 分频更新占空比（每10ms）
    if(led->div_cnt >= BREATH_DIV)
    {
        led->div_cnt = 0;

        // LUT查表
        led->lut_idx++;
        if(led->lut_idx >= LUT_SIZE) led->lut_idx = 0;
        led->rear_duty = breath_lut[led->lut_idx];
    }

    // PWM周期溢出（每10ms = 100 * 0.1ms）
    if(led->pwm_cnt >= PWM_PERIOD)
        led->pwm_cnt = 0;

    // 输出比较
    if(led == &Led_Group[LED_R_GROUP])
    {
        if(led->pwm_cnt < led->rear_duty)
            RH_LED_ON();
        else
            RH_LED_OFF();
    }
    else if(led == &Led_Group[LED_L_GROUP])
    {
        if(led->pwm_cnt < led->rear_duty)
            LH_LED_ON();
        else
            LH_LED_OFF();
    }
}

//==================== 结构体实例化 ====================
LED_Object_t Led_Group[3] =
        {
                {0},    // 下标0 占位
                {0, 0, 0, 1, 1,0, LED_Group_Breath_Scan, LED_Rear_Breath_Set},
                {0, 0, 0, 1, 1, 0,LED_Group_Breath_Scan, LED_Rear_Breath_Set}
        };

//==================== 系统初始化 ====================
void LED_System_Init(void)
{
    Led_Group[LED_R_GROUP].Rear_Breath_Set(&Led_Group[LED_R_GROUP], 1);
    Led_Group[LED_L_GROUP].Rear_Breath_Set(&Led_Group[LED_L_GROUP], 1);
}