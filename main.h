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
		u16 Cell_OVP : 1;	   // 单体过压
		u16 Cell_UVP : 1;	   // 单体欠压
		u16 Bat_OVP : 1;	   // 电池过压
		u16 Bat_UVP : 1;	   // 电池欠压
		u16 Charge_OTP : 1;	   // 充电过温
		u16 Charge_UTP : 1;	   // 充电低温
		u16 DisCharge_OTP : 1; // 放电过温
		u16 DisCharge_UTP : 1; // 放电低温
		u16 Charge_OCP : 1;	   // 充电过流
		u16 DisCharge_OCP : 1; // 放电过流
		u16 Bat_Short : 1;	   // 电池短路
		u16 IC_Err : 1;		   // 前端检测IC错误
		u16 Soft_Lock : 1;	   // 软件锁定MOS
		u16 Ambient_OTP : 1;   // 环境过温
		u16 Ambient_UTP : 1;   // 环境低温
		u16 Fet_OTP : 1;	   // FET过温
	} Bit;
};

union Alarm
{
	u16 all;
	struct
	{
		u16 Cell_UVA : 1;	   // 单体低压告警
		u16 Cell_OVA : 1;	   // 单体高压告警
		u16 Bat_UVA : 1;	   // 整组低压告警
		u16 Bat_OVA : 1;	   // 整组高压告警
		u16 Charge_OCA : 1;	   // 充电过流告警
		u16 DisCharge_OCA : 1; // 放电过流告警
		u16 Charge_OTA : 1;	   // 充电高温告警
		u16 Charge_UTA : 1;	   // 充电低温告警
		u16 DisCharge_OTA : 1; // 放电高温告警
		u16 DisCharge_UTA : 1; // 放电低温告警
		u16 Ambient_OTA : 1;   // 环境高温告警
		u16 Ambient_UTA : 1;   // 环境低温告警
		u16 PCB_OTA : 1;	   // PCB高温告警
		u16 VolDiff_Big : 1;   // 压差大告警
		u16 Capacity_Low : 1;  // 容量低告警
		u16 reserve : 1;	   // res
	} Bit;
};

typedef struct
{
	u16 Bat_V;						// 2BYTE总电压,单位 10mV，高字节在前，下同
	int16_t Bat_I;					// 2BYTE电流，单位 10mA，充电为正，放电为负。
	u16 Surplus_Capacity;			// 2BYTE剩余容量，单位 10mAh
	u16 Nominal_Capacity;			// 2BYTE标称容量，单位 10mAh
	u16 Cycle_Cnt;					// 2BYTE循环次数
	u16 Production_Date;			// 2BYTE生产日期，，采用 2 个字节传送比如 0x2068,其中日期为最低 5 为：0x2028&0x1f = 8 表示日期;月份（0x2068>>5）&0x0f = 0x03 表示 3 月;年份就为 2000+ (0x2068>>9) = 2000 + 0x10 =2016;
	u16 Equalize_Sta_L;				// 2BYTE低16位1~16串，均衡状态 0关闭  1打开
	u16 Equalize_Sta_H;				// 2BYTE高16位17~31串，均衡状态 0关闭  1打开
	union BAT_FAULT Fault_Sta;		// 2BYTE故障状态
	u8 Soft_Ver;					// 1BYTE软件版本号
	u8 RSOC;						// 1BYTE剩余容量百分比
	u8 Fet_Sta;						// 1BYTE MOS指示状态,bit0 表示充电,bit1 表示放电,0 表示 MOS 关闭,1 表示打开
	union Alarm Alarm_Sta;			// 2BYTE 告警状态, 每一个bit表示一种告警状态，0为未发生，1发生告警
	u16 Ambient_Temp;				// 2 byte 环境温度 单位0.1K， 采用绝对温度传输，2731+（实际温度*10）,0度 = 2731  25度 = 2731+25*10 = 2981
	u16 Fet_Temp;					// 2 byte FET温度 单位0.1K, 同上
	u8 NTC_Num;						// 1 byte NTC个数
	int16_t NTC_Value[BMS_NTC_MAX]; // NTC内容 单位0.1K, 同上
	u8 Cell_Num;					// 电池数量
	u16 Cell_Vol[BMS_CELL_MAX];		// 单体电压
} JBD_BMS;

typedef struct
{
	s8 DisplayTask_ID;		// 显示任务ID
	s8 CommuToDCTask_ID;	// DC通讯任务ID
	s8 CommuToBMSTask_ID;	// BMS通讯任务ID
	u8 BMS_Receive_Timeout; // BMS通讯超时
	u8 BMS_Receive_Error;	// BMS通讯错误
	u8 BMS_Send_Flag;		// BMS发送标志
	u8 DC_Receive_Timeout;	// DC通讯超时
	u8 DC_Receive_Error;	// DC通讯错误
	u8 BMS_Rx[BMS_Rx_MAX];	// BMS接收缓存
	u8 BMS_Rx_Pos;			// BMS接收缓存下标
	u8 DC_Rx[DC_Rx_MAX];	// DC接收缓存
	u8 DC_Rx_Pos;			// DC接收缓存下标
	u8 DC_Tx[DC_Tx_MAX];	// DC发送缓存
	u8 DC_Tx_Pos;			// DC发送缓存下标
	u8 ChargingFlag;		// 充电中标志, 只能赋值0或1
	u8 ReChargingFlag;		// 重新充电中标志, 只能赋值0或1
	u8 BMS_ID;				// BMS ID
	JBD_BMS BMS_INFO;		// BMS信息
} Global_Var;

extern Global_Var Global;

void lcd_init(void);
void display_all(void);

void mainDisplay(void);
void ShowRSOC(u8 n);
void ShowErrorAndCharges(u8 n);
void ShowNoNum(u8 n);
