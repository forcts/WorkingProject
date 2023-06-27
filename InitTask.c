#include "main.h"

void CommuToBMSTask(void);

void CommuToDCTask(void)
{
}

void DisplayTask(void)
{
	ShowRSOC(Global.BMS_INFO.RSOC);
	ShowErrorAndCharges(Global.BMS_Errors);
	ShowNoNum(Global.BMS_ID);
}

void InitTask(void)
{
	InitTaskQueue((taskType *)&taskQueue);

	if (Create_task(&CommuToBMSTask, PERIOD_500MS, 0, PRIORITY_1) < 0)
		return;
	if (Create_task(&CommuToDCTask, PERIOD_2MS, 0, PRIORITY_1) < 0)
		return;
	if (Create_task(&DisplayTask, PERIOD_100MS, 0, PRIORITY_1) < 0)
		return;
}
