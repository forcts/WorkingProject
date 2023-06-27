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
	else if (Global.BMS_Receive_Timeout)
		ShowErrorAndCharges(2); // BMS通讯超时
	else if (Global.DC_Receive_Timeout)
		ShowErrorAndCharges(3); // DC通讯超时
	else if (Global.DC_Receive_Error)
		ShowErrorAndCharges(4); // DC通讯错误
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
