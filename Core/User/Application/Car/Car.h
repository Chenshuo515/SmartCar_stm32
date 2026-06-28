//
// Created by 陈烁 on 24-6-9.
//

#ifndef CAR_CONTROL_H
#define CAR_CONTROL_H
#include "Pid/PID.h"
#include "../../config.h"
#include "../../Modules/Motor/motor.h"

extern motor_data_t Motor[2]; //0为左轮，1为右轮

void Car_Init();

void Car_goStright(int speed);

void Car_conDirection(int leftspeed,int rightspeed);

void Car_stop();
void Car_TurnSelf(int Speed);


void Car_Auto_Control(float normalized_error);

void Car_turnAngle_pidCal(float current_angle,float target_angle);


void Car_Move_With_Turn(float base_speed,int current_angle,int target_angle);
/* 边走边转向控制（弧线运动） */
#endif //CAR_CONTROL_H
