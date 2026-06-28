#ifndef __LED_H
#define __LED_H

#include "stm32f1xx_hal.h"

// 分组：右组(右前+右后)、左组(左前+左后)
typedef enum
{
    LED_R_GROUP  = 1,
    LED_L_GROUP  = 2
} LED_Group_t;

// PWM与呼吸参数
#define PWM_PERIOD      100    // 100us * 20 = 2ms 完整PWM周期 500HZ
#define BREATH_DIV      100     // 每100次扫描(10ms)才更新一次亮度，放慢呼吸
#define MAX_DUTY      160    // 最大占空比70%，预留灭灯区间，不会常亮

// 前向声明
typedef struct _LED_Object LED_Object_t;

// LED结构体
struct _LED_Object
{
    uint8_t pwm_cnt;        // PWM周期计数器
    uint8_t div_cnt;        // 呼吸分频计数器
    float rear_duty;      // 后灯PWM占空比 0~PWM_PERIOD
    uint8_t breath_dir;     // 1=渐亮  0=渐暗
    uint8_t rear_breath_en; // 后灯呼吸使能
    uint8_t lut_idx;

    // 方法指针
    void (*Breath_Scan)(LED_Object_t *led);
    void (*Rear_Breath_Set)(LED_Object_t *led, uint8_t en);
};

extern LED_Object_t Led_Group[3];

// 底层接口
void RQ_LED_ON(void);
void RQ_LED_OFF(void);
void RH_LED_ON(void);
void RH_LED_OFF(void);
void ALLR_LED_ON(void);
void ALLR_LED_OFF(void);

void LQ_LED_ON(void);
void LQ_LED_OFF(void);
void LH_LED_ON(void);
void LH_LED_OFF(void);
void ALLL_LED_ON(void);
void ALLL_LED_OFF(void);

// 系统初始化
void LED_System_Init(void);

#endif