#include "main.h"
#include "string.h"
#include "schedularAlgorithm.h"

Global_Var Global; // 全局变量
JBD_BMS BMS_INFO;  // BMS信息

void InitControl(void);
void InitTask(void);

int main(void)
{
	memset(&Global, 0, sizeof(Global_Var));
	memset(&BMS_INFO, 0, sizeof(JBD_BMS));
	InitControl();
	InitTask();
	while(1)
	{
		TimeBaseHandle();
		HandleSchedular();
	}
}
