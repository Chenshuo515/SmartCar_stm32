
// track.h
#ifndef __TRACK_H
#define __TRACK_H

#include "../Motor/motor.h"
#include "Pid/PID.h"
#include "stdbool.h"
#include "../../Application/Task/Task.h"
#include "../../config.h"



//双环控制参数
//#define XJ_BASE_SPEED      3.5f   // 基础速度
//#define MAX_TURN        3.3f   // 最大转向调整量
//#define LOST_LINE_TIMEOUT 50    // 丢失线路计数超时

extern bool permanent_loss;   // 永久丢失标志
extern bool permanent_arrivel;
extern bool permanent_Serch;   // 循迹中标志位
extern uint16_t line_analog_avg[9];

void Track_Init();
void Five_line_GetDate();
float line_following_control();



#endif