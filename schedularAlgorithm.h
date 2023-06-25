/*************************************************
Copyright (C), 2018-2028,    YWS
File name: schedularAlgorithm.h
Author:  
Version:  
Custom version: 
Date: 2022/8/21
Description: It provide the data structure and functions for schecular


Others:
Function List:
        void initTaskQueue(taskType *ptr_taskQueue)
            This function is used to initialze the task queue.
                Note: The first element in the task queue will be load a sysErroReport() function,
                      it run every 5 minutes play the role of collecting the system error.

        int create_task(fp ptr_task, Uint32 period, Uint32 start_time, Uint8 priority)
            This function can create a task which will be push into taskqueue

1. History:

2. Date:
Author:
Modification:
***************************************************/


#ifndef _SCHEDULARALGORITHM_H
#define _SCHEDULARALGORITHM_H

#include "timebase.h"

#define NO_TASK_ID       (-1)

#define TaskPeriod_Us(x) (schedular_UsToTicks(x)-schedular_UsToTicks(20))/*error value*/
#define TaskPeriod_Ms(x) (schedular_MsToTicks(x)-schedular_UsToTicks(30))/*error value*/

#define PERIOD_1US        TaskPeriod_Us(1)
#define PERIOD_50US       TaskPeriod_Us(50)
#define PERIOD_100US      TaskPeriod_Us(100)
#define PERIOD_200US      TaskPeriod_Us(200)
#define PERIOD_300US      TaskPeriod_Us(300)
#define PERIOD_400US      TaskPeriod_Us(400)
#define PERIOD_500US      TaskPeriod_Us(500)
#define PERIOD_650US      TaskPeriod_Us(650)
#define PERIOD_700US      TaskPeriod_Us(700)
#define PERIOD_1P2MS      TaskPeriod_Us(1200)
#define PERIOD_1P3MS      TaskPeriod_Us(1300)
#define PERIOD_1P5MS      TaskPeriod_Us(1500)
#define PERIOD_1MS        TaskPeriod_Ms(1)
#define PERIOD_2MS        TaskPeriod_Ms(2)
#define PERIOD_3MS        TaskPeriod_Ms(3)
#define PERIOD_4MS        TaskPeriod_Ms(4)
#define PERIOD_5MS        TaskPeriod_Ms(5)
#define PERIOD_6MS        TaskPeriod_Ms(6)
#define PERIOD_7MS        TaskPeriod_Ms(7)
#define PERIOD_10MS       TaskPeriod_Ms(10)
#define PERIOD_20MS       TaskPeriod_Ms(20)
#define PERIOD_30MS       TaskPeriod_Ms(30)
#define PERIOD_50MS       TaskPeriod_Ms(50)
#define PERIOD_60MS       TaskPeriod_Ms(60)
#define PERIOD_100MS      TaskPeriod_Ms(100)
#define PERIOD_200MS      TaskPeriod_Ms(200)
#define PERIOD_400MS      TaskPeriod_Ms(400)
#define PERIOD_500MS      TaskPeriod_Ms(500)
#define PERIOD_1S      	  TaskPeriod_Ms(1000)


#ifndef _taskId_t
#define _taskId_t
typedef signed char taskId_t;
#endif


enum Task_Priority{
	PRIORITY_0,//user app do not use it, it's for kernel
	PRIORITY_1,
	PRIORITY_2,
	PRIORITY_3,
	PRIORITY_4,
	PRIORITY_5,
	PRIORITY_6,
	PRIORITY_7,
	PRIORITY_8,
	PRIORITY_9,
	PRIORITY_10,
	PRIORITY_11,
	PRIORITY_12,
	PRIORITY_13,
	PRIORITY_14,
	PRIORITY_15,
	PRIORITY_16,
	PRIORITY_17,
	PRIORITY_18,
	PRIORITY_19,
	PRIORITY_20,

	TASK_NUM
};

#define READY_TASK_NUM (TASK_NUM-3)


typedef struct tag_timeBaseType {
    uint32_t timer;	// 计时器
	uint32_t ticks;	// 滴答时钟
    uint32_t period;	// 周期
}time_baseType;

#ifndef _fp
#define _fp
typedef void(*fp)(void);
#endif

typedef struct Tag_taskType{
    fp ptr_task;
	time_baseType timeBase;
	//bool trigger;
	//Uint8 priority;
}taskType;

extern taskType taskQueue[TASK_NUM];
extern fp ready_taskQueue[READY_TASK_NUM];


extern void InitTaskQueue(taskType *ptr_taskQueue);
extern int Create_task(fp ptr_task, uint32_t period, uint32_t start_time, uint8_t priority);
extern void Delete_task(taskId_t *id);
extern void HandleSchedular(void);
extern void TaskPeriodIncrease(taskId_t id, uint32_t time);
extern void TaskPeriodDecrease(taskId_t id, uint32_t time);
extern void TaskPeriodSet(taskId_t id, uint32_t time);
extern void TimeBaseHandle(void);

#endif
