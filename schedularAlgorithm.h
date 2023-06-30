#ifndef _SCHEDULARALGORITHM_H
#define _SCHEDULARALGORITHM_H

#include "n32g430_tim.h"

/*The longest time:(2^16)*1/48MHZ*/
#define SCHEDULAR_COUNTER_MSK 0x0000FFFF
#define CONFIG_SCHEDULAR_HZ 48000000 // select timer3    96Mhz
typedef unsigned long Ticks_t;

#define ROUND_TO(type, val) ((type)((val) < 0 ? (val)-0.5 : (val) + 0.5))
#define schedular_SecToTicksUserType(type, sec) ROUND_TO(type, (double)CONFIG_SCHEDULAR_HZ *(sec))
#define schedular_MsToTicksUserType(type, ms) ROUND_TO(type, (double)CONFIG_SCHEDULAR_HZ *(ms) / 1000.0)
#define schedular_UsToTicksUserType(type, us) ROUND_TO(type, (double)CONFIG_SCHEDULAR_HZ *(us) / 1000000.0)
#define schedular_SecToTicks(sec) schedular_SecToTicksUserType(Ticks_t, sec)
#define schedular_MsToTicks(ms) schedular_MsToTicksUserType(Ticks_t, ms)
#define schedular_UsToTicks(us) schedular_UsToTicksUserType(Ticks_t, us)

static inline Ticks_t scehdular_ElapsedTicket(Ticks_t *p_start_time)
{
	Ticks_t now = SCHEDULAR_COUNTER_MSK - TIM3->CNT; // The systick timer is a down counter
	if (*p_start_time == 0)							 // Restart counter
		*p_start_time = now;
	Ticks_t st_Elapsed = 0;
	st_Elapsed = (now - *p_start_time) & SCHEDULAR_COUNTER_MSK;
	*p_start_time = now;
	return (st_Elapsed & SCHEDULAR_COUNTER_MSK);
}

#define NO_TASK_ID (-1)

#define TaskPeriod_Us(x) (schedular_UsToTicks(x) - schedular_UsToTicks(20)) /*error value*/
#define TaskPeriod_Ms(x) (schedular_MsToTicks(x) - schedular_UsToTicks(30)) /*error value*/

#define PERIOD_1US TaskPeriod_Us(1)
#define PERIOD_50US TaskPeriod_Us(50)
#define PERIOD_100US TaskPeriod_Us(100)
#define PERIOD_200US TaskPeriod_Us(200)
#define PERIOD_300US TaskPeriod_Us(300)
#define PERIOD_400US TaskPeriod_Us(400)
#define PERIOD_500US TaskPeriod_Us(500)
#define PERIOD_650US TaskPeriod_Us(650)
#define PERIOD_700US TaskPeriod_Us(700)
#define PERIOD_1P2MS TaskPeriod_Us(1200)
#define PERIOD_1P3MS TaskPeriod_Us(1300)
#define PERIOD_1P5MS TaskPeriod_Us(1500)
#define PERIOD_1MS TaskPeriod_Ms(1)
#define PERIOD_2MS TaskPeriod_Ms(2)
#define PERIOD_3MS TaskPeriod_Ms(3)
#define PERIOD_4MS TaskPeriod_Ms(4)
#define PERIOD_5MS TaskPeriod_Ms(5)
#define PERIOD_6MS TaskPeriod_Ms(6)
#define PERIOD_7MS TaskPeriod_Ms(7)
#define PERIOD_10MS TaskPeriod_Ms(10)
#define PERIOD_20MS TaskPeriod_Ms(20)
#define PERIOD_30MS TaskPeriod_Ms(30)
#define PERIOD_50MS TaskPeriod_Ms(50)
#define PERIOD_60MS TaskPeriod_Ms(60)
#define PERIOD_100MS TaskPeriod_Ms(100)
#define PERIOD_200MS TaskPeriod_Ms(200)
#define PERIOD_400MS TaskPeriod_Ms(400)
#define PERIOD_500MS TaskPeriod_Ms(500)
#define PERIOD_1S TaskPeriod_Ms(1000)

#ifndef _taskId_t
#define _taskId_t
typedef signed char taskId_t;
#endif

enum Task_Priority
{
	PRIORITY_0, // user app do not use it, it's for kernel
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

#define READY_TASK_NUM (TASK_NUM - 3)

typedef struct tag_timeBaseType
{
	uint32_t timer;	 // 计时器
	uint32_t ticks;	 // 滴答时钟
	uint32_t period; // 周期
} time_baseType;

#ifndef _fp
#define _fp
typedef void (*fp)(void);
#endif

typedef struct Tag_taskType
{
	fp ptr_task;
	time_baseType timeBase;
} taskType;

extern taskType taskQueue[TASK_NUM];
extern fp ready_taskQueue[READY_TASK_NUM];

extern void InitTaskQueue(taskType *ptr_taskQueue);
extern int Create_task(fp ptr_task, uint32_t period, uint8_t priority);
extern void Delete_task(taskId_t *id);
extern void HandleSchedular(void);
extern void TaskPeriodSet(taskId_t id, uint32_t time);
extern void TimeBaseHandle(void);

#endif
