//
// Created by Gleam on 26-6-21.
//

#include "Task.h"


/* ========== 任务函数定义 ========== */
//当前电池电压
static float volt;
// 电池电压读取 (10ms)
static void BatVolt_LED_Task(void)
{

    //电池电压扫描
    BatVolt_Scan();
    volt= BatVolt_Get_Voltage();
    if(volt < 4.7)
    {
        //开启呼吸灯
        Led_Group[LED_R_GROUP].Rear_Breath_Set(&Led_Group[LED_R_GROUP],1);
        Led_Group[LED_L_GROUP].Rear_Breath_Set(&Led_Group[LED_L_GROUP],1);

    }
    else
    {
        Led_Group[LED_R_GROUP].Rear_Breath_Set(&Led_Group[LED_R_GROUP],0);
        Led_Group[LED_L_GROUP].Rear_Breath_Set(&Led_Group[LED_L_GROUP],0);
    }
}
// 音乐状态机 (10ms) —— 如果已用硬件定时器，这里只负责切换音符
static void Buzzer_Music_Task(void)
{

    Buzzer_Start_StarSong();
    //状态机
    Buzzer_Song_State_Machine();
}


static float XJ_Motor_PWM;
// 巡线 PID 控制 (10ms)
static void line_following_Pid_Task(void) {

    //五路循迹模块读取
    Five_line_GetDate();
    //正在循迹标志位
    permanent_Serch = true;
    //获取PID计算值
    XJ_Motor_PWM = line_following_control();

}

static float Distance;
static  bool permanent_avoid = false;   // 避障标志
// 超声波状态机 (30ms)
static void Ultrasonund_Task(void)
{
    //超声波测距状态机
    Ultrasonic_Timer_ISR_Handler();
    Distance = Ultrasonic_Get_Distance();
    if(Distance < MIN_DISTENCE)
    {
        permanent_avoid = true;
    }
    else
    {
        permanent_avoid = false;
    }
}

//小车行驶任务
static void Car_Control_Task(void)
{
    if(permanent_avoid == true)
    {
        Car_TurnSelf(TurnSelf_SPEED);
    }
        //当正在循迹且未完全丢失路线时
    else if(permanent_Serch == true && permanent_loss == false)
    {
        Motor[0].Speed_out  = XJ_BASE_SPEED - XJ_Motor_PWM * MAX_TURN;
        Motor[1].Speed_out = XJ_BASE_SPEED + XJ_Motor_PWM * MAX_TURN;
        //此次循迹完成，标志位置0
        permanent_Serch = false;
    }
    else
    {
        Car_stop();
    }
    Car_conDirection((int )Motor[0].Speed_out,(int)Motor[1].Speed_out);

}



// OLED 刷新 (100ms)
static void OLED_Update_Task(void)
{
    // OLED显示更新（每100ms更新一次）

        // OLED_ShowSignedNum(3, 6, Motor[0].Speed_real, 2);
        OLED_ShowNum(1,11,Key_cnt ,1);//显示当前赛题
        OLED_ShowNum(2, 1, line_analog_avg[1], 1);
        OLED_ShowNum(2, 3, line_analog_avg[2], 1);
        OLED_ShowNum(2, 5, line_analog_avg[3], 1);
        OLED_ShowNum(2, 7, line_analog_avg[4], 1);
        OLED_ShowNum(2, 9, line_analog_avg[5], 1);

        OLED_ShowNum(2,11,volt*10,2);

        OLED_ShowSignedNum(3, 5, Motor[0].Speed_out, 3);
        OLED_ShowSignedNum(3, 10, Motor[1].Speed_out, 3);
        OLED_ShowSignedNum(4, 1, Flag_10ms_Cnt, 4);
        OLED_ShowSignedNum(4, 7, main_cnt, 3);
        OLED_ShowSignedNum(4, 12, Distance, 3);

        // OLED_ShowSignedNum(4, 6, cx[1], 3);
        // OLED_ShowSignedNum(4, 12, cy[1], 3);
}




static int id_BatVolt_LED, id_XJ_PID, id_ultra, id_buzzer, id_oled,id_Car;
/* 注册所有任务并保存 ID */
void Task_All_Init(void) {
    id_BatVolt_LED  = Task_Register(BatVolt_LED_Task, 50);
    id_XJ_PID   = Task_Register(line_following_Pid_Task, 10);
    id_ultra   = Task_Register(Ultrasonund_Task,    10);
    id_Car    = Task_Register(Car_Control_Task, 10);
    id_buzzer  = Task_Register(Buzzer_Music_Task,  10);
    id_oled    = Task_Register(OLED_Update_Task,   100);

}

/* 开关函数的实现（ID 对外不可见） */
void Task_BatVolt_LED_Enable(bool on) {
    if (on) Task_Enable(id_BatVolt_LED); else Task_Disable(id_BatVolt_LED);
}
void Task_line_following_Pid_Enable(bool on) {
    if (on) Task_Enable(id_XJ_PID); else Task_Disable(id_XJ_PID);
}
void Task_Ultrasonund_Enable(bool on) {
    if (on) Task_Enable(id_ultra); else Task_Disable(id_ultra);
}

void Task_Music_Enable(bool on) {
    if (on) Task_Enable(id_buzzer); else Task_Disable(id_buzzer);
}
void Task_Car_Pidcontrol_Enable(bool on) {
    if (on) Task_Enable(id_Car); else Task_Disable(id_Car);
}
void Task_OLED_Enable(bool on) {
    if (on) Task_Enable(id_oled); else Task_Disable(id_oled);
}
