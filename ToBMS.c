#include "main.h"

static const u8 Send[][10] = {
	{0xDD, 0x00, 0xA5, 0x04, 0x00, 0xFF, 0x57, 0x77},			  // ��ȡ�����ѹ
	{0xDD, 0x00, 0xA5, 0x03, 0x00, 0xFF, 0x58, 0x77},			  // ��ȡBMS������Ϣ
	{0xDD, 0x00, 0xA5, 0x05, 0x00, 0xFF, 0x56, 0x77},			  // ��������
	{0xDD, 0x00, 0x5A, 0xE1, 0x02, 0x00, 0x00, 0xFE, 0xC3, 0x77}, // ��ŵ翪
	{0xDD, 0x00, 0x5A, 0xE1, 0x02, 0x00, 0x01, 0xFE, 0xC2, 0x77}, // ���طŵ翪
	{0xDD, 0x00, 0x5A, 0xE1, 0x02, 0x00, 0x02, 0xFE, 0xC1, 0x77}, // ��翪�ŵ��
	{0xDD, 0x00, 0x5A, 0xE1, 0x02, 0x00, 0x03, 0xFE, 0xC0, 0x77}  // ��ŵ��
};

enum Send_Flags
{
	Get_Cell_Info_Flag = 0, // ��ȡBMS������Ϣ
	Get_BMS_Info_Flag,		// ��ȡ�����ѹ
	Get_Wake_Info_Flag,		// ��������
	CHG_DSG_ON_Flag,		// ��ŵ翪
	CHG_OFF_DSG_ON_Flag,	// ���طŵ翪
	CHG_ON_DSG_OFF_Flag,	// ��翪�ŵ��
	CHG_DSG_OFF_Flag,		// ��ŵ��
};

void CommuToBMSTask(void)
{
	if (++Global.BMS_Receive_Timeout > 5) // BMSͨѶ��ʱ
	{
		Global.BMS_Receive_Error = 1;
		Global.BMS_Send_Flag = ++Global.BMS_Send_Flag % 3; // ͨѶ��ʱ�������ֻ�����Ϣ
		TaskPeriodSet(Global.CommuToBMSTask_ID, PERIOD_200MS);
	}
	if (Global.BMS_INFO.Soft_Ver != 0) // ͨѶ�����򽵵�ͨѶ�ٶ�
		TaskPeriodSet(Global.CommuToBMSTask_ID, PERIOD_1S);
	else
		TaskPeriodSet(Global.CommuToBMSTask_ID, PERIOD_200MS);
	static u8 i, k;
	GPIO_Pins_Set(GPIOB, GPIO_PIN_1); // ʹ��485оƬ����
	if (Global.BMS_Send_Flag < 3)
		k = 9;
	else
		k = 11;
	for (i = 0; i < k; i++)
	{
		USART_Data_Send(USART2, Send[Global.BMS_Send_Flag][i]);
		while (USART_Flag_Status_Get(USART2, USART_FLAG_TXDE) == RESET)
			;
	}
	GPIO_Pins_Reset(GPIOB, GPIO_PIN_1); // ʹ��485оƬ����
}

u8 BMS_Rx[BMS_Rx_MAX];		 // BMS���ջ���
void USART2_IRQHandler(void) // �����ж�
{
	// static u8 BMS_Rx[BMS_Rx_MAX]; // BMS���ջ���
	static u8 BMS_Rx_Pos; // BMS���ջ����±�
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
		/* У�� */
		u16 S = 0, j;
		for (j = 2; j < BMS_Rx[4] + 5; j++)
			S += BMS_Rx[j];
		if ((BMS_Rx[BMS_Rx_Pos - 2] == ((((~S) + 1) & 0xff00) >> 8)) && (BMS_Rx[BMS_Rx_Pos - 1] == (((~S) + 1) & 0x00ff)))
		{
			/*��ȡ*/
			j = 5;
			if (Global.BMS_Send_Flag == Get_Cell_Info_Flag)
			{
				Global.BMS_INFO.Cell_Num = BMS_Rx[4] >> 1;
				for (S = 0; j < BMS_Rx_Pos - 3;)
				{
					Global.BMS_INFO.Cell_Vol[S++] = (BMS_Rx[j] << 8) | BMS_Rx[j + 1];
					j += 2;
				}
			}
			else if (Global.BMS_Send_Flag == Get_BMS_Info_Flag)
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
			else
			{
				if (BMS_Rx[3] == 0 && BMS_Rx_Pos == 7) // ���Ƴɹ�
					; // do something
				else
					; // do something
			}
			/*��ԭ*/
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
