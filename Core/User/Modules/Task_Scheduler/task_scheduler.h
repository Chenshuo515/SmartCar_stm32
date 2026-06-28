//
// Created by Gleam on 26-6-21.
//
#ifndef __TASK_SCHEDULER_H
#define __TASK_SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TASKS  10   // 最大任务数量，可按需修改

typedef void (*TaskFunc_t)(void);   // 任务函数指针

typedef struct {
    TaskFunc_t func;         // 任务回调函数
    uint32_t   interval;     // 调用间隔（单位：10ms节拍），0表示单次任务
    uint32_t   next_run;     // 下次执行的节拍值
    bool       enable;       // 使能标志
} Task_t;



/* 初始化任务管理器（主程序初始化时调用一次） */
void Task_Init(void);

/* 注册一个任务，period_ms：周期（毫秒），设为0为单次任务，返回任务ID（失败返回-1） */
int Task_Register(TaskFunc_t func, uint16_t period_ms);

/* 禁止指定任务 */
void Task_Disable(uint8_t id);

/* 使能指定任务，并重新设定其下次执行时间 */
void Task_Enable(uint8_t id);

/* 调度器运行函数：在主循环中调用，检查并执行所有到期任务 */
void Task_Scheduler_Run(void);

#ifdef __cplusplus
}
#endif

#endif
