#include "main.h"
#include "string.h"
#include "schedularAlgorithm.h"

Global_Var Global; // 全局变量

int main(void)
{
	memset(&Global, 0, sizeof(Global_Var));
	InitControl();
	InitTask();
	while(1)
	{
		TimeBaseHandle();
		HandleSchedular();
	}
}
