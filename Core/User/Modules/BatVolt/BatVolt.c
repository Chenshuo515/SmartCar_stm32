//
// Created by Gleam on 26-6-19.
//

#include "BatVolt.h"

// 滤波缓冲区
static uint16_t Batadc_buf[FILTER_SAMPLE] = {0};
static uint8_t  buf_index = 0;
static float    bat_vol = 0.0f;

/**
 * @brief  读取ADC1_IN1原始值
 * @retval ADC原始数值 0~4095
 */
static uint16_t ADC_Bat_Read(void)
{

    HAL_ADC_Start(&hadc1);
    // 超时2ms，中断内安全，不会阻塞
    if(HAL_ADC_PollForConversion(&hadc1, 2) == HAL_OK)
    {
        uint16_t val = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        return val;
    }
    HAL_ADC_Stop(&hadc1);
    return 0;
}

/**
 * @brief  电池电压扫描，10ms中断里调用
 */
void BatVolt_Scan(void)
{
    // 存入采样缓冲区
    Batadc_buf[buf_index] = adc_buf[0];
    buf_index = (buf_index + 1) % FILTER_SAMPLE;

    // 计算平均值
    uint32_t sum = 0;
    for(uint8_t i = 0; i < FILTER_SAMPLE; i++)
    {
        sum += Batadc_buf[i];
    }
    uint16_t adc_avg = sum / FILTER_SAMPLE;

    // 换算真实电池电压
    float adc_voltage = adc_avg * ADC_REF_VOLT / ADC_MAX_VAL;
    bat_vol = adc_voltage * BAT_DIV_RATIO;
}

/**
 * @brief  获取当前电池电压
 * @retval 电池电压 float
 */
float BatVolt_Get_Voltage(void)
{
    return bat_vol;
}