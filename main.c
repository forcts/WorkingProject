#include "main.h"
#include "string.h"
#include "schedularAlgorithm.h"

Global_Var Global; // 全局变量

void InitTask(void);
void InitControl(void);
void CommuToBMSTask(void);
void CommuToDCTask(void);

int main(void)
{
	memset(&Global, 0, sizeof(Global_Var));
	InitControl();
	InitTask();
	while (1)
	{
		TimeBaseHandle();
		HandleSchedular();
	}
}

void DisplayTask(void)
{
	if (Global.BMS_Receive_Error) // BMS通讯错误
	{
		memset(&Global.BMS_INFO, 0, sizeof(JBD_BMS)); // BMS信息清空
		ShowErrorAndCharges(1);
	}
	else if (Global.BMS_Control_Failed > 3)
	{
		Global.BMS_Send_Flag = ++Global.BMS_Send_Flag % 2;
		ShowErrorAndCharges(1);
	}
	else if (Global.DC_Receive_Error) // DC通讯错误
	{
		ShowErrorAndCharges(3);
	}
	else // 无错误
	{
		ShowErrorAndCharges(0);
	}
	ShowRSOC(Global.BMS_INFO.RSOC);
	ShowNoNum(Global.BMS_ID);
}

void InitTask(void)
{
	InitTaskQueue((taskType *)&taskQueue);
	Global.CommuToBMSTask_ID = Create_task(&CommuToBMSTask, PERIOD_200MS, PRIORITY_1);
	Global.CommuToDCTask_ID = Create_task(&CommuToDCTask, PERIOD_50MS, PRIORITY_1);
	Global.DisplayTask_ID = Create_task(&DisplayTask, PERIOD_200MS, PRIORITY_1);
	if (Global.CommuToBMSTask_ID < 0 || Global.CommuToDCTask_ID < 0 || Global.DisplayTask_ID < 0)
		return;
}
