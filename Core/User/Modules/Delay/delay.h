//
// Created by ³ÂË¸ on 25-7-13.
//

#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f1xx_hal.h"
extern volatile uint32_t nowtime;
void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

#endif