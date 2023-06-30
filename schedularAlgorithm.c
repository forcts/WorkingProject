#include <stdio.h>
#include "schedularAlgorithm.h"

taskType taskQueue[TASK_NUM];
fp ready_taskQueue[READY_TASK_NUM];

void SysErroReport(void)
{
	return;
}

void InitTaskQueue(taskType *ptr_taskQueue)
{
	uint8_t i = 1;
	ptr_taskQueue[0].ptr_task = &SysErroReport;
	ptr_taskQueue[0].timeBase.period = 0xffffffff; // was schedular_SecToTicks(300); and schedular_SecToTicks(300) > 0xffffffff
	ptr_taskQueue[0].timeBase.ticks = 0;
	ptr_taskQueue[0].timeBase.timer = 0;
	while (i < TASK_NUM)
	{
		ptr_taskQueue[i].ptr_task = (fp)NULL;
		ptr_taskQueue[i].timeBase.period = 0;
		ptr_taskQueue[i].timeBase.ticks = 0;
		ptr_taskQueue[i].timeBase.timer = 0;
		i++;
	}
	i = 0;
	while (i < READY_TASK_NUM)
	{
		ready_taskQueue[i++] = NULL;
	}
}

int Create_task(fp ptr_task, uint32_t period, uint8_t priority)
{
	int i = priority;
	for (; i < TASK_NUM; i++)
	{
		if (NULL == taskQueue[i].ptr_task)
		{
			taskQueue[i].ptr_task = ptr_task;
			taskQueue[i].timeBase.period = period;
			taskQueue[i].timeBase.timer = 0;
			taskQueue[i].timeBase.ticks = 0;
			return i;
		}
	}

	return -1;
}

void Delete_task(taskId_t *task_ID)
{
	if (*task_ID < 0)
	{
		return;
	}

	taskQueue[*task_ID].ptr_task = (fp)NULL;
	taskQueue[*task_ID].timeBase.period = 0;
	taskQueue[*task_ID].timeBase.timer = 0;
	taskQueue[*task_ID].timeBase.ticks = 0;
	*task_ID = NO_TASK_ID;
}

void TaskPeriodSet(taskId_t id, uint32_t time)
{
	taskQueue[id].timeBase.period = time;
}

void HandleSchedular(void)
{
	uint8_t i = 0;
	while (ready_taskQueue[i] != NULL)
	{
		(ready_taskQueue[i])();
		ready_taskQueue[i] = NULL;
		i++;
	}
}

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
				taskQueue[i].timeBase.timer = 0;
				taskQueue[i].timeBase.ticks = 0;
				ready_taskQueue[(k++) % READY_TASK_NUM] = taskQueue[i].ptr_task;
			}
		}
		i++;
	}
}
