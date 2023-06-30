#include "n32g430.h"
#include "schedularAlgorithm.h"
#include "n32g430_gpio.h"

#define LCD_SCK_PIN             GPIO_PIN_9
#define LCD_SCK_SET_H           GPIO_Pins_Set(GPIOA,LCD_SCK_PIN)
#define LCD_SCK_SET_L           GPIO_Pins_Reset(GPIOA,LCD_SCK_PIN)
#define LCD_SCK_GET             GPIO_Input_Data_Get(GPIOA,LCD_SCK_PIN)

#define LCD_SDA_PIN             GPIO_PIN_10
#define LCD_SDA_SET_H           GPIO_Pins_Set(GPIOA,LCD_SDA_PIN)
#define LCD_SDA_SET_L           GPIO_Pins_Reset(GPIOA,LCD_SDA_PIN)
#define LCD_SDA_GET             GPIO_Input_Data_Get(GPIOA,LCD_SDA_PIN)

#define LCD_RST_PIN             GPIO_PIN_1
#define LCD_RST_SET_H           GPIO_Pins_Set(GPIOA,LCD_RST_PIN)
#define LCD_RST_SET_L           GPIO_Pins_Reset(GPIOA,LCD_RST_PIN)

#define LCD_BL_PIN              GPIO_PIN_7
#define LCD_BL_SET_H            GPIO_Pins_Set(GPIOA,LCD_BL_PIN)
#define LCD_BL_SET_L            GPIO_Pins_Reset(GPIOA,LCD_BL_PIN)
#define LCD_BL_SIGNAl           GPIO_Input_Data_Get(GPIOA,LCD_BL_PIN)
#define LCD_BL_TOGGLE           GPIO_Pin_Toggle(GPIOA, LCD_BL_PIN)

#define LCD_CS_HIGH LCD_RST_SET_H
#define LCD_CS_LOW LCD_RST_SET_L

#define LCD_WR_HIGH LCD_SCK_SET_H
#define LCD_WR_LOW LCD_SCK_SET_L

#define LCD_DATA_HIGH LCD_SDA_SET_H
#define LCD_DATA_LOW LCD_SDA_SET_L

#define LCD_BL_ON LCD_BL_SET_H
#define LCD_BL_OFF LCD_BL_SET_L

#define BMS_NTC_MAX 10
#define BMS_CELL_MAX 32
#define BMS_Rx_MAX 100
#define DC_Rx_MAX 100
#define DC_Tx_MAX 100

union BAT_FAULT
{
	u16 all;
	struct
	{
		u16 Cell_OVP : 1;	   // �����ѹ
		u16 Cell_UVP : 1;	   // ����Ƿѹ
		u16 Bat_OVP : 1;	   // ��ع�ѹ
		u16 Bat_UVP : 1;	   // ���Ƿѹ
		u16 Charge_OTP : 1;	   // ������
		u16 Charge_UTP : 1;	   // ������
		u16 DisCharge_OTP : 1; // �ŵ����
		u16 DisCharge_UTP : 1; // �ŵ����
		u16 Charge_OCP : 1;	   // ������
		u16 DisCharge_OCP : 1; // �ŵ����
		u16 Bat_Short : 1;	   // ��ض�·
		u16 IC_Err : 1;		   // ǰ�˼��IC����
		u16 Soft_Lock : 1;	   // �������MOS
		u16 Ambient_OTP : 1;   // ��������
		u16 Ambient_UTP : 1;   // ��������
		u16 Fet_OTP : 1;	   // FET����
	} Bit;
};

union Alarm
{
	u16 all;
	struct
	{
		u16 Cell_UVA : 1;	   // �����ѹ�澯
		u16 Cell_OVA : 1;	   // �����ѹ�澯
		u16 Bat_UVA : 1;	   // �����ѹ�澯
		u16 Bat_OVA : 1;	   // �����ѹ�澯
		u16 Charge_OCA : 1;	   // �������澯
		u16 DisCharge_OCA : 1; // �ŵ�����澯
		u16 Charge_OTA : 1;	   // �����¸澯
		u16 Charge_UTA : 1;	   // �����¸澯
		u16 DisCharge_OTA : 1; // �ŵ���¸澯
		u16 DisCharge_UTA : 1; // �ŵ���¸澯
		u16 Ambient_OTA : 1;   // �������¸澯
		u16 Ambient_UTA : 1;   // �������¸澯
		u16 PCB_OTA : 1;	   // PCB���¸澯
		u16 VolDiff_Big : 1;   // ѹ���澯
		u16 Capacity_Low : 1;  // �����͸澯
		u16 reserve : 1;	   // res
	} Bit;
};

typedef struct
{
	u16 Bat_V;						// 2BYTE�ܵ�ѹ,��λ 10mV�����ֽ���ǰ����ͬ
	int16_t Bat_I;					// 2BYTE��������λ 10mA�����Ϊ�����ŵ�Ϊ����
	u16 Surplus_Capacity;			// 2BYTEʣ����������λ 10mAh
	u16 Nominal_Capacity;			// 2BYTE�����������λ 10mAh
	u16 Cycle_Cnt;					// 2BYTEѭ������
	u16 Production_Date;			// 2BYTE�������ڣ������� 2 ���ֽڴ��ͱ��� 0x2068,��������Ϊ��� 5 Ϊ��0x2028&0x1f = 8 ��ʾ����;�·ݣ�0x2068>>5��&0x0f = 0x03 ��ʾ 3 ��;��ݾ�Ϊ 2000+ (0x2068>>9) = 2000 + 0x10 =2016;
	u16 Equalize_Sta_L;				// 2BYTE��16λ1~16��������״̬ 0�ر�  1��
	u16 Equalize_Sta_H;				// 2BYTE��16λ17~31��������״̬ 0�ر�  1��
	union BAT_FAULT Fault_Sta;		// 2BYTE����״̬
	u8 Soft_Ver;					// 1BYTE����汾��
	u8 RSOC;						// 1BYTEʣ�������ٷֱ�
	u8 Fet_Sta;						// 1BYTE MOSָʾ״̬,bit0 ��ʾ���,bit1 ��ʾ�ŵ�,0 ��ʾ MOS �ر�,1 ��ʾ��
	union Alarm Alarm_Sta;			// 2BYTE �澯״̬, ÿһ��bit��ʾһ�ָ澯״̬��0Ϊδ������1�����澯
	u16 Ambient_Temp;				// 2 byte �����¶� ��λ0.1K�� ���þ����¶ȴ��䣬2731+��ʵ���¶�*10��,0�� = 2731  25�� = 2731+25*10 = 2981
	u16 Fet_Temp;					// 2 byte FET�¶� ��λ0.1K, ͬ��
	u8 NTC_Num;						// 1 byte NTC����
	int16_t NTC_Value[BMS_NTC_MAX]; // NTC���� ��λ0.1K, ͬ��
	u8 Cell_Num;					// �������
	u16 Cell_Vol[BMS_CELL_MAX];		// �����ѹ
} JBD_BMS;

typedef struct
{
	s8 DisplayTask_ID;		// ��ʾ����ID
	s8 CommuToDCTask_ID;	// DCͨѶ����ID
	s8 CommuToBMSTask_ID;	// BMSͨѶ����ID
	u8 BMS_Receive_Timeout; // BMSͨѶ��ʱ
	u8 BMS_Receive_Error;	// BMSͨѶ����
	u8 BMS_Send_Flag;		// BMS���ͱ�־
	u8 DC_Receive_Timeout;	// DCͨѶ��ʱ
	u8 DC_Receive_Error;	// DCͨѶ����
	u8 BMS_Rx[BMS_Rx_MAX];	// BMS���ջ���
	u8 BMS_Rx_Pos;			// BMS���ջ����±�
	u8 DC_Rx[DC_Rx_MAX];	// DC���ջ���
	u8 DC_Rx_Pos;			// DC���ջ����±�
	u8 DC_Tx[DC_Tx_MAX];	// DC���ͻ���
	u8 DC_Tx_Pos;			// DC���ͻ����±�
	u8 ChargingFlag;		// ����б�־, ֻ�ܸ�ֵ0��1
	u8 ReChargingFlag;		// ���³���б�־, ֻ�ܸ�ֵ0��1
	u8 BMS_ID;				// BMS ID
	JBD_BMS BMS_INFO;		// BMS��Ϣ
} Global_Var;

extern Global_Var Global;

void lcd_init(void);
void display_all(void);

void mainDisplay(void);
void ShowRSOC(u8 n);
void ShowErrorAndCharges(u8 n);
void ShowNoNum(u8 n);
