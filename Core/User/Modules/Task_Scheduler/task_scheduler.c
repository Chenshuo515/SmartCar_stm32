//
// Created by Gleam on 26-6-21.
//
#include "task_scheduler.h"

Task_t task_list[MAX_TASKS];
uint8_t task_count = 0;

// 用户需在自己的10ms定时器中断里递增此变量
volatile uint32_t Flag_10ms_Cnt = 0;

/**
 * @brief  初始化任务管理器
 */
void Task_Init(void)
{
    task_count = 0;
    for (int i = 0; i < MAX_TASKS; i++) {
        task_list[i].func     = NULL;
        task_list[i].interval = 0;
        task_list[i].next_run = 0;
        task_list[i].enable   = false;
    }
}

/**
 * @brief  注册一个任务,初始失能 需手动使能
 * @param  func       任务函数指针
 * @param  period_ms  调用周期（毫秒），10ms对齐，0表示单次执行
 * @return 成功返回任务ID（0 ~ MAX_TASKS-1），失败返回 -1
 */
int Task_Register(TaskFunc_t func, uint16_t period_ms)
{
    if (task_count >= MAX_TASKS) return -1;
    if (func == NULL) return -1;

    uint32_t ticks = period_ms / 10;
    if (ticks == 0 && period_ms > 0) {
        ticks = 1;   // 最小间隔 10ms
    }

    task_list[task_count].func     = func;
    task_list[task_count].interval = ticks;
    task_list[task_count].next_run = Flag_10ms_Cnt + ticks;
    task_list[task_count].enable   = false;

    return task_count++;
}

/**
 * @brief  禁止指定任务
 * @param  id  任务ID
 */
void Task_Disable(uint8_t id)
{
    if (id < task_count) {
        task_list[id].enable = false;
    }
}

/**
 * @brief  使能指定任务，并重新安排其执行时间
 * @param  id  任务ID
 */
void Task_Enable(uint8_t id)
{
    if (id < task_count) {
        /*只有从禁用状态变为启用时，才重置下次执行时间 */
        if (!task_list[id].enable) {
            task_list[id].next_run = Flag_10ms_Cnt + task_list[id].interval;
        }
        task_list[id].enable = true;
    }
}

/**
 * @brief  调度器主函数，每次调用时执行所有到期任务
 */
void Task_Scheduler_Run(void)
{
    for (int i = 0; i < task_count; i++) {
        if (!task_list[i].enable) continue;

        if (Flag_10ms_Cnt >= task_list[i].next_run) {
            // 执行任务
            task_list[i].func();

            if (task_list[i].interval > 0)
            {
                // 周期任务：累加间隔，保持长期节拍
                task_list[i].next_run += task_list[i].interval;
                // 如果因执行时间过长导致严重落后，复位到当前时刻，避免连续多次执行
                if (Flag_10ms_Cnt >= task_list[i].next_run) {
                    task_list[i].next_run = Flag_10ms_Cnt + task_list[i].interval;
                }
            }
            else
            {
                // 单次任务执行后自动禁止
                task_list[i].enable = false;
            }
        }
    }
}
