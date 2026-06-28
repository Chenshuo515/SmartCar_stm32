//
// Created by Gleam on 26-6-19.
//

#ifndef __BAT_VOLT_H
#define __BAT_VOLT_H

#include "stm32f1xx_hal.h"
#include "adc.h"
#include "../../Application/Task/Task.h"

// 配置参数，根据硬件修改
#define ADC_REF_VOLT    3.3f      // 参考电压
#define ADC_MAX_VAL     4095     // 12位ADC最大值
#define BAT_DIV_RATIO   3.0f     // 分压比例 R总/R下
#define FILTER_SAMPLE   4        // 均值滤波采样点数

// 函数声明
void BatVolt_Scan(void);        // 在10ms定时器中断中调用
float BatVolt_Get_Voltage(void);// 获取当前电池电压

#endif