#include "main.h"

static const u8 Get_BMS_Info[] = {0xDD, 0x00, 0xA5, 0x03, 0x00, 0xFF, 0x58, 0x77};	// 读取BMS基本信息
static const u8 Get_Cell_Info[] = {0xDD, 0x00, 0xA5, 0x04, 0x00, 0xFF, 0x57, 0x77}; // 读取单体电压
static const u8 Get_Wake_Info[] = {0xDD, 0x00, 0xA5, 0x05, 0x00, 0xFF, 0x56, 0x77}; // 唤醒作用

void CommuToBMSTask(void)
{
	if (++Global.BMS_Receive_Timeout > 5) // BMS通讯超时
		Global.BMS_Receive_Error = 1;
	if (Global.BMS_Receive_Error == 1) // 通讯超时则发送三种唤醒信息
		Global.BMS_Send_Flag = ++Global.BMS_Send_Flag % 3;
	if (Global.BMS_INFO.Soft_Ver != 0) // 通讯正常则降低通讯速度
		TaskPeriodSet(Global.CommuToBMSTask_ID, PERIOD_1S);
	else
		TaskPeriodSet(Global.CommuToBMSTask_ID, PERIOD_200MS);
	static u8 i;
	GPIO_Pins_Set(GPIOB, GPIO_PIN_1); // 使能485芯片发送
	for (i = 0; i < 9; i++)
	{
		if (Global.BMS_Send_Flag == 0) // 读取单体电压
			USART_Data_Send(USART2, Get_Cell_Info[i]);
		else if (Global.BMS_Send_Flag == 1) // 读取BMS基本信息
			USART_Data_Send(USART2, Get_BMS_Info[i]);
		else
			USART_Data_Send(USART2, Get_Wake_Info[i]);
		while (USART_Flag_Status_Get(USART2, USART_FLAG_TXDE) == RESET)
			;
	}
	GPIO_Pins_Reset(GPIOB, GPIO_PIN_1); // 使能485芯片接收
}

void USART2_IRQHandler(void) // 接收中断
{
	static u8 BMS_Rx[BMS_Rx_MAX]; // BMS接收缓存
	static u8 BMS_Rx_Pos;		  // BMS接收缓存下标
	Global.BMS_Receive_Timeout = 0;
	BMS_Rx[BMS_Rx_Pos] = USART2->DAT;
	if ((BMS_Rx[0] != 0xDD) || (BMS_Rx_Pos > BMS_Rx_MAX - 2))
	{
		BMS_Rx_Pos = 0;
		USART_Flag_Clear(USART2, USART_INT_RXDNE);
		return;
	}
	if (BMS_Rx[BMS_Rx_Pos] == 0x77)
	{
		/* 校验 */
		u16 S = 0, j;
		for (j = 2; j < BMS_Rx[4] + 5; j++)
			S += BMS_Rx[j];
		if ((BMS_Rx[BMS_Rx_Pos - 2] == ((((~S) + 1) & 0xff00) >> 8)) && (BMS_Rx[BMS_Rx_Pos - 1] == (((~S) + 1) & 0x00ff)))
		{
			/*提取*/
			j = 5;
			if (Global.BMS_Send_Flag == 0)
			{
				Global.BMS_INFO.Cell_Num = BMS_Rx[4] >> 1;
				for (S = 0; j < BMS_Rx_Pos - 3;)
				{
					Global.BMS_INFO.Cell_Vol[S++] = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
					j += 2;
				}
			}
			else if (Global.BMS_Send_Flag == 1)
			{
				Global.BMS_INFO.Bat_V = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.Bat_I = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.Surplus_Capacity = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.Nominal_Capacity = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.Cycle_Cnt = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.Production_Date = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.Equalize_Sta_L = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.Equalize_Sta_H = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.Fault_Sta.all = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.Soft_Ver = BMS_Rx[j++];
				Global.BMS_INFO.RSOC = BMS_Rx[j++];
				Global.BMS_INFO.Fet_Sta = BMS_Rx[j++];
				Global.BMS_INFO.Cell_Num = BMS_Rx[j++];
				Global.BMS_INFO.Alarm_Sta.all = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.Ambient_Temp = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.Fet_Temp = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
				j += 2;
				Global.BMS_INFO.NTC_Num = BMS_Rx[j++];
				for (S = 0; j < BMS_Rx_Pos - 2;)
				{
					Global.BMS_INFO.NTC_Value[S++] = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
					j += 2;
				}
			}
			/*还原*/
			Global.BMS_Send_Flag = ++Global.BMS_Send_Flag % 2;
			Global.BMS_Receive_Error = 0;
		}
		else
			Global.BMS_Receive_Error = 1;
		BMS_Rx_Pos = 0;
		USART_Flag_Clear(USART2, USART_INT_RXDNE);
		return;
	}

	BMS_Rx_Pos++;
	USART_Flag_Clear(USART2, USART_INT_RXDNE);
}
