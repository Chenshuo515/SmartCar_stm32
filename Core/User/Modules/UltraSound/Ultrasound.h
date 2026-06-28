#ifndef __ULTRASOUND_H
#define __ULTRASOUND_H

#include "stm32f1xx_hal.h"
#include "../OLED/Dwt.h"
#include "../../Application/Task/Task.h"
// 超声波状态机枚举
typedef enum {
    US_IDLE,
    US_TRIGGER,
    US_WAIT_ECHO,
    US_CALCULATE
} US_StateTypeDef;




// 函数声明
void Ultrasonic_Timer_ISR_Handler(void);
void Ultrasonic_Start_Measure(void);
float Ultrasonic_Get_Distance(void);

#endif