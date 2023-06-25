#include "main.h"

static const u8 Get_BMS_Info[] = {0xDD, 0x00, 0xA5, 0x03, 0x00, 0xFF, 0x58, 0x77};	// 读取BMS基本信息
static const u8 Get_Cell_Info[] = {0xDD, 0x00, 0xA5, 0x04, 0x00, 0xFF, 0x57, 0x77}; // 读取单体电压

void CommuToBMSTask(void)
{
	if (++Global.BMS_Receive_Timeout > 10)
	{
		// BMS通讯超时
		Global.BMS_Receive_Error = 1;
	}

	static u8 i = 0;
	for (i = 0; i < 8; i++)
	{
		while (USART_Flag_Status_Get(USART2, USART_FLAG_TXC) == RESET)
			;
		if (Global.BMS_Send_Flag) // 读取BMS基本信息
		{
			USART2->DAT = Get_BMS_Info[i];
		}
		else // 读取单体电压
		{
			USART2->DAT = Get_Cell_Info[i];
		}
	}
}

void DecodeBMS(void)
{
	/* 校验 */
	if ((Global.BMS_Rx[1] != 0x00) || (Global.BMS_Rx[1] != 0x00) || (Global.BMS_Rx[2] != 4 - Global.BMS_Send_Flag))
	{
		Global.BMS_Receive_Error = 1;
		return;
	}
	u16 S = 0, j;
	for (j = 2; j < Global.BMS_Rx[4] + 4; j++)
		S += Global.BMS_Rx[j];
	S = (~S) + 1;
	if ((Global.BMS_Rx[Global.BMS_Rx_Pos - 3] != ((S & 0xff00) >> 8)) || (Global.BMS_Rx[Global.BMS_Rx_Pos - 2] != (S & 0x00ff)))
	{
		Global.BMS_Receive_Error = 1;
		return;
	}

	/*提取*/
	j = 5;
	if (Global.BMS_Send_Flag)
	{
		BMS_INFO.Bat_V = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j += 2;
		BMS_INFO.Bat_I = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j += 2;
		BMS_INFO.Surplus_Capacity = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j += 2;
		BMS_INFO.Nominal_Capacity = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j += 2;
		BMS_INFO.Cycle_Cnt = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j += 2;
		BMS_INFO.Production_Date = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j += 2;
		BMS_INFO.Equalize_Sta_L = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j += 2;
		BMS_INFO.Equalize_Sta_H = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j += 2;
		BMS_INFO.Fault_Sta.all = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j++;
		BMS_INFO.Soft_Ver = Global.BMS_Rx[j++];
		BMS_INFO.RSOC = Global.BMS_Rx[j++];
		BMS_INFO.Fet_Sta = Global.BMS_Rx[j++];
		BMS_INFO.Cell_Num = Global.BMS_Rx[j++];
		BMS_INFO.Alarm_Sta.all = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j += 2;
		BMS_INFO.Ambient_Temp = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j += 2;
		BMS_INFO.Fet_Temp = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
		j++;
		BMS_INFO.NTC_Num = Global.BMS_Rx[j++];
		for (S = 0; j < Global.BMS_Rx_Pos - 3; j++)
		{
			BMS_INFO.NTC_Value[S++] = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
			j++;
		}
	}
	else
	{
		BMS_INFO.Cell_Num = Global.BMS_Rx[j] >> 1;
		for (S = 0; j < Global.BMS_Rx_Pos - 3; j++)
		{
			BMS_INFO.Cell_Vol[S++] = (Global.BMS_Rx[j] << 8) | Global.BMS_Rx[j + 1];
			j++;
		}
	}

	/*还原*/
	Global.BMS_Receive_Error = 0;
	Global.BMS_Send_Flag = Global.BMS_Send_Flag++ % 2;
}

void USART2_IRQHandler(void) // 接收中断
{
	Global.BMS_Receive_Timeout = 0;
	Global.BMS_Rx[Global.BMS_Rx_Pos] = USART2->DAT;
	if ((Global.BMS_Rx[0] != 0xDD) || (Global.BMS_Rx_Pos > BMS_Rx_MAX - 2))
	{
		Global.BMS_Rx_Pos = 0;
		USART_Flag_Clear(USART2, USART_INT_RXDNE);
		return;
	}
	if (Global.BMS_Rx[Global.BMS_Rx_Pos] == 0x77)
	{
		DecodeBMS();
		Global.BMS_Rx_Pos = 0;
		USART_Flag_Clear(USART2, USART_INT_RXDNE);
		return;
	}

	Global.BMS_Rx_Pos++;
	USART_Flag_Clear(USART2, USART_INT_RXDNE);
}
