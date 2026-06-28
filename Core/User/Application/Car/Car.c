//
// Created by 陈烁 on 24-6-9.
//
#include "Car.h"

motor_data_t Motor[2]; //0为左轮，1为右轮



static pid_config_t Motor0Speed_config = INIT_PID_CONFIG(Motor0Speed_KP_V, Motor0Speed_KI_V, Motor0Speed_KD_V, Motor0Speed_INTEGRAL_V, Motor0Speed_MAX_V,
                                                         (PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement));


static pid_config_t Motor1Speed_config = INIT_PID_CONFIG(Motor1Speed_KP_V, Motor1Speed_KI_V, Motor1Speed_KD_V, Motor1Speed_INTEGRAL_V, Motor1Speed_MAX_V,
                                                         (PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement));

static  pid_obj_t *CarSelfTurn_pid;  // 自转PID控制器
static pid_config_t CarSelfTurn_pid_config = INIT_PID_CONFIG(CarSelfTurn_KP_A, CarSelfTurn_KI_A, CarSelfTurn_KD_A,CarSelfTurn_INTEGRAL_A, CarSelfTurn_MAX_A,
                                                             (PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement));

static  pid_obj_t *CarTurn_pid;  // 转向PID控制器
static pid_config_t CarTurn_pid_config = INIT_PID_CONFIG(CarTurn_KP_V, CarTurn_KI_V, CarTurn_KD_V,CarTurn_INTEGRAL_V, CarTurn_MAX_V,
                                                         (PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement));

void Car_Init()
{
    //注册并初始化左右轮电机PID
    Motor[0].MotorSpeed_pid = pid_register(&Motor0Speed_config);
    Motor[1].MotorSpeed_pid = pid_register(&Motor1Speed_config);

    //差速转向pid
    CarTurn_pid = pid_register(&CarTurn_pid_config);
    //自转pid
    CarSelfTurn_pid = pid_register(&CarSelfTurn_pid_config);

}
void Car_goStright(int speed)
{
    Motor_SetLeftSpeed(speed);
    Motor_SetRightSpeed(speed);
}

void Car_conDirection(int leftspeed,int rightspeed)
{
    Motor_SetLeftSpeed(leftspeed);
    Motor_SetRightSpeed(rightspeed);
}


// 停止电机
void Car_stop()
{
    Motor[0].Speed_out  = 0;
    Motor[1].Speed_out = 0;

}

void Car_TurnSelf(int Speed)
{
    Motor[0].Speed_out  = -Speed;
    Motor[1].Speed_out = Speed;
}


void Car_turnAngle_pidCal(float current_angle,float target_angle)
{
    float SelfTurn_output = pid_calculate(CarSelfTurn_pid,current_angle,target_angle);
    Motor[0].Speed_ref = SelfTurn_output;
    Motor[1].Speed_ref = -SelfTurn_output;
}

/* 边走边转向控制（弧线运动） */
void Car_Move_With_Turn(float base_speed,int current_angle,int target_angle)
{
    float turn_correction = pid_calculate(CarTurn_pid, current_angle, target_angle);

    Motor[0].Speed_ref = base_speed + turn_correction;
    Motor[1].Speed_ref = base_speed - turn_correction;
}
