#include "schedularAlgorithm.h"

void CommuToBMSTask(void);

void CommuToDCTask(void)
{
}

void DisplayTask(void)
{
}

void InitTask(void)
{
	InitTaskQueue((taskType *)&taskQueue);

	if (Create_task(&CommuToBMSTask, PERIOD_500MS, 0, PRIORITY_1) < 0)
		return;
	if (Create_task(&CommuToDCTask, PERIOD_2MS, 0, PRIORITY_1) < 0)
		return;
	if (Create_task(&DisplayTask, PERIOD_2MS, 0, PRIORITY_1) < 0)
		return;
}
