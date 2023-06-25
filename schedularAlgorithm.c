/*************************************************
Copyright (C), 2018-2028,  YWS. Co., Ltd.
File name: schedularAlgorithm.c
Author: Chen wei
Version: V0001
Custom version: V0001
Date: 2018/8/21
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
#include <stdio.h>
#include "schedularAlgorithm.h"

taskType taskQueue[TASK_NUM];
fp ready_taskQueue[READY_TASK_NUM];

/*******************************************************************************
 * Function: sysErroReport(void)
 *
 * Calls:
 *
 * Calls By: schedular
 *
 * Input:
 *
 * Output:
 *
 * Description: Collecting the system error
 *
 * Notes:It locate at the first place in the task queue
 *******************************************************************************/
void SysErroReport(void)
{
	return;
}

/*******************************************************************************
 * Function: void initTaskQueue(taskType *ptr_taskQueue)
 *
 * Calls:
 *
 * Calls By: void InitTask(void)
 *
 * Input: taskType *ptr_taskQueue, the task queue's address
 *
 * Output:
 *
 * Description: load the sysErroReport task into the first place, and initializing
 * all of the array
 *
 * Notes:
 *******************************************************************************/
void InitTaskQueue(taskType *ptr_taskQueue)
{
	uint8_t i = 1;

	ptr_taskQueue[0].ptr_task = &SysErroReport;
	ptr_taskQueue[0].timeBase.period = schedular_SecToTicks(300);
	ptr_taskQueue[0].timeBase.ticks = 0;
	ptr_taskQueue[0].timeBase.timer = 0;
	// ptr_taskQueue[0].trigger = FALSE;

	while (i < TASK_NUM)
	{
		ptr_taskQueue[i].ptr_task = (fp)NULL;
		ptr_taskQueue[i].timeBase.period = 0;
		ptr_taskQueue[i].timeBase.ticks = 0;
		ptr_taskQueue[i].timeBase.timer = 0;
		// ptr_taskQueue[i].trigger = FALSE;
		i++;
	}
	i = 0;
	while (i < READY_TASK_NUM)
	{
		ready_taskQueue[i++] = NULL;
	}
}

/*******************************************************************************
 * Function: int create_task(fp ptr_task, Uint32 period, Uint32 init_time, Uint8 priority)
 *
 * Calls:
 *
 * Calls By: void InitTask(void), void CtrlMachineStateTask(void)
 *
 * Input: ptr_task for task name, period represent the running time gap, init_time
 * indicate the start time, task will be place at "priority" number at arrry.
 *
 * Output: once a task is created successfully, function will return the id place,
 * or it will return -1
 *
 * Description: there maybe exist a task already in the pointing place, then it will search
 * the next available room in the task queue, and return the id, it return -1 when out of the range
 *
 *
 * Notes:
 *******************************************************************************/
int Create_task(fp ptr_task, uint32_t period, uint32_t init_time, uint8_t priority)
{
	int i = priority;
	for (; i < TASK_NUM; i++)
	{
		if (NULL == taskQueue[i].ptr_task)
		{
			taskQueue[i].ptr_task = ptr_task;
			taskQueue[i].timeBase.period = period;
			taskQueue[i].timeBase.timer = period - init_time; // start frome it
			taskQueue[i].timeBase.ticks = 0;
			return i;
		}
	}

	return -1;

#if 0
	if ((priority > 0)&&(priority < TASK_NUM) && (NULL == taskQueue[priority].ptr_task))//
   {
       taskQueue[priority].ptr_task = ptr_task;
	   taskQueue[priority].timeBase.period = period;
	   taskQueue[priority].timeBase.timer = period - init_time;//start frome it
	   return priority;
   }
   return -1;
#endif
}

/*******************************************************************************
 * FUnction: void delete_task(taskId_t task_ID)
 *
 * Calls:
 *
 * Calls By: void CtrlMachineStateTask(void)
 *
 * Input: task id
 *
 * Output:
 *
 * Description: make the fuction point to NULL
 *
 *
 * Notes:
 *******************************************************************************/
void Delete_task(taskId_t *task_ID)
{
	if (*task_ID < 0)
	{
		return;
	}

	taskQueue[*task_ID].ptr_task = (fp)NULL;
	taskQueue[*task_ID].timeBase.period = 0;
	taskQueue[*task_ID].timeBase.timer = 0; // start from it
	taskQueue[*task_ID].timeBase.ticks = 0;
	// taskQueue[*task_ID].trigger = FALSE;
	*task_ID = NO_TASK_ID; // indicating that it a not-use ID
}

/*******************************************************************************
 * Function: void taskPeriodIncrease(taskId_t id, Uint32 time)
 *
 * Calls:
 *
 * Calls By:
 *
 * Input: task id, time
 *
 * Output:
 *
 * Description: can make the period longer
 *
 *
 * Notes:
 *******************************************************************************/
void TaskPeriodIncrease(taskId_t id, uint32_t time)
{
	taskQueue[id].timeBase.period += time;
}

void TaskPeriodSet(taskId_t id, uint32_t time)
{
	taskQueue[id].timeBase.period = time;
}

/*******************************************************************************
 * Function: void taskPeriodDecrease(taskId_t id, Uint32 time)
 *
 * Calls:
 *
 * Calls By:
 *
 * Input: task id, time
 *
 * Output:
 *
 * Description: can make the period shorter
 *
 *
 * Notes:
 *******************************************************************************/
void TaskPeriodDecrease(taskId_t id, uint32_t time)
{
	if (taskQueue[id].timeBase.period < time)
		taskQueue[id].timeBase.period = 0;
	else
		taskQueue[id].timeBase.period -= time;
}

/*******************************************************************************
 * void handleSchedular(void)
 *
 * Calls:
 *
 * Calls By: main
 *
 * Input:
 *
 * Output:
 *
 * Description: schedule the get-ready task to run
 *
 *
 * Notes:
 *******************************************************************************/
void HandleSchedular(void)
{
	uint8_t i = 0;
	// fp taskRun = (fp)NULL;
	while (ready_taskQueue[i] != NULL)
	{
		(ready_taskQueue[i])();
		ready_taskQueue[i] = NULL;
		i++;
	}
}

// #pragma  CODE_SECTION (TimeBaseHandle, "ramfuncs");
void TimeBaseHandle(void)
{
	uint8_t i = 0;
	uint8_t k = 0;
	while (i < TASK_NUM)
	{
		if (taskQueue[i].ptr_task != NULL)
		{
			taskQueue[i].timeBase.timer += scehdular_ElapsedTicket((Ticks_t *)(&(taskQueue[i].timeBase.ticks)));
			if (taskQueue[i].timeBase.timer > taskQueue[i].timeBase.period)
			{
				// taskQueue[i].trigger = TRUE;
				taskQueue[i].timeBase.timer = 0;
				taskQueue[i].timeBase.ticks = 0;
				// ready_taskQueue[k++] = taskQueue[i].ptr_task;
				ready_taskQueue[(k++) % READY_TASK_NUM] = taskQueue[i].ptr_task;
			}
		}
		i++;
	}
}
