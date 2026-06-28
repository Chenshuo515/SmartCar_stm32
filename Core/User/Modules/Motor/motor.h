//
// Created by 陈烁 on 24-6-8.
//
#include "main.h"
#include "../../config.h"
#include "Pid/PID.h"

#ifndef MOTOR_H
#define MOTOR_H


typedef struct
{
    short EncodeCount;    //编码器1计数值
    float Speed_real; //实际速度
    float Speed_ref;  //目标速度
    float  Speed_out ;  //输出pwm占空比(-100~100)
    pid_obj_t *MotorSpeed_pid;

}motor_data_t;

void Motor_SetLQSpeed(int speed);

void Motor_SetLHSpeed(int speed);

void Motor_SetRQSpeed(int speed);

//右后
void Motor_SetRHSpeed(int speed);

void Motor_SetLeftSpeed(int Motor1Speed);

void Motor_SetRightSpeed(int Motor2Speed);




#endif //MOTOR_H
