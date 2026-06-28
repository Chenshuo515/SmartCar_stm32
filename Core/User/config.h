//
// Created by 陈烁 on 25-7-10.
//
#include "Pid/PID.h"

#ifndef CONFIG_H
#define CONFIG_H


//循迹数据
#define XJ_BASE_SPEED      50   // 基础速度
//传感器权重
#define FIVESOR_WEIGHTS {  -2.0, -1.0, 0, 1.0, 2.0}
// 巡线控制参数
#define MAX_TURN          32   // 最大转向调整量
#define LOST_LINE_TIMEOUT 300    // 丢失线路计数超时
#define total_eror  3.0f

//超声波避障最近距离
#define  MIN_DISTENCE 50
//自转速度
#define TurnSelf_SPEED 40

//520电机最大速度约为6.4r/s
#define Motor0Speed_KP_V         40
#define Motor0Speed_KI_V         35
#define Motor0Speed_KD_V         0
#define Motor0Speed_INTEGRAL_V   70
#define Motor0Speed_MAX_V        100

#define Motor1Speed_KP_V         40
#define Motor1Speed_KI_V         35
#define Motor1Speed_KD_V         0
#define Motor1Speed_INTEGRAL_V   70
#define Motor1Speed_MAX_V        100

#define CarTurn_KP_V         2
#define CarTurn_KI_V         0
#define CarTurn_KD_V         0.002f
#define CarTurn_INTEGRAL_V   0
#define CarTurn_MAX_V        3

#define CarSelfTurn_KP_A         0.5
#define CarSelfTurn_KI_A         0
#define CarSelfTurn_KD_A         0.002f
#define CarSelfTurn_INTEGRAL_A   0
#define CarSelfTurn_MAX_A        3

#define eightline_search_KP         1.05f
#define eightline_search_KI         0
#define eightline_search_KD         0.01f
#define eightline_search_INTEGRAL   0    //积分项
#define eightline_search_MAX        2    //最大输出值





#endif //CONFIG_H
