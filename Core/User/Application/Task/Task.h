//
// Created by Gleam on 26-6-21.
//

#ifndef __TASK_H
#define __TASK_H

#include <stdint.h>
#include "../../Modules/Task_Scheduler/task_scheduler.h"
#include "../../Modules/Track/track.h"        // 假设你有这些模块的接口
#include "Pid/PID.h"
#include "../../Modules/UltraSound/Ultrasound.h"
#include "../../Modules/Key/Key.h"
#include "../../Modules/Buzzer/Buzzer.h"
#include "../../Modules/OLED/drv_OLED.h"
#include "../Car/Car.h"
#include "../../Modules/BatVolt/BatVolt.h"
#include "../../Modules/LED/LED.h"

//从外部获取
extern int8_t Key_cnt;
extern uint16_t main_cnt;
extern  uint16_t adc_buf[6];



void Task_All_Init(void );
/* 开关函数的实现（ID 对外不可见） */
void Task_BatVolt_LED_Enable(bool on) ;
void Task_line_following_Pid_Enable(bool on) ;
void Task_Ultrasonund_Enable(bool on);
void Task_Music_Enable(bool on) ;
void Task_Car_Pidcontrol_Enable(bool on);
void Task_OLED_Enable(bool on) ;
// ...

#endif