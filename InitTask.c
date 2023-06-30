#include "main.h"

void CommuToBMSTask(void);

void CommuToDCTask(void)
{
}

void DisplayTask(void)
{
	ShowRSOC(Global.BMS_INFO.RSOC);
	ShowNoNum(Global.BMS_ID);
	if (Global.BMS_Receive_Error)
		ShowErrorAndCharges(1); // BMS通讯错误
	else if (Global.DC_Receive_Error)
		ShowErrorAndCharges(2); // DC通讯错误
	else
		ShowErrorAndCharges(0); // 无错误
}

void InitTask(void)
{
	InitTaskQueue((taskType *)&taskQueue);
	Global.CommuToBMSTask_ID = Create_task(&CommuToBMSTask, PERIOD_200MS, PRIORITY_1);
	Global.CommuToDCTask_ID = Create_task(&CommuToDCTask, PERIOD_2MS, PRIORITY_1);
	Global.DisplayTask_ID = Create_task(&DisplayTask, PERIOD_200MS, PRIORITY_1);
	if (Global.CommuToBMSTask_ID < 0 || Global.CommuToDCTask_ID < 0 || Global.DisplayTask_ID < 0)
		return;
}
