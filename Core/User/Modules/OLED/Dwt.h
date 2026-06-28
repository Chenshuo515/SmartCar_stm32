//
// Created by Gleam on 26-5-12.
//

#ifndef SMARTCAR_DWT_H
#define SMARTCAR_DWT_H

#endif //SMARTCAR_DWT_H
#ifndef __DRV_DWT_H
#define __DRV_DWT_H

#include "stm32f1xx_hal.h"

void dwt_init(uint32_t cpu_freq_mhz);
void dwt_systime_update(void);

uint64_t dwt_get_time_us(void);
float    dwt_get_time_ms(void);
float    dwt_get_time_s(void);

float    dwt_get_delta(uint32_t *cnt_last);
double   dwt_get_delta_64(uint32_t *cnt_last);

void     dwt_delay_s(float delay);
// 微秒级延时
void dwt_delay_us(uint32_t us);

// 毫秒级延时
void dwt_delay_ms(uint32_t ms);
#endif