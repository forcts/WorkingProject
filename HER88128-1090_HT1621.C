#include <LCDCtrol.h>
#include <main.h>

// 依次显示出        	0    1    2    3    4    5    6    7    8    9

u8 data_table1[] = {0x0F, 0x00, 0x0B, 0x09, 0x04, 0x0D, 0x0F, 0x08, 0x0F, 0x0D}; // AFED数码8421
u8 data_table2[] = {0x0A, 0x0A, 0x0C, 0x0E, 0x0E, 0x06, 0x06, 0x0A, 0x0E, 0x0E}; // BGC*数码8421

void DELAY(long int t)
{
	int i, j;
	for (i = 0; i < t; i++)
		for (j = 0; j < 10; j++)
			;
}

void WRITE_COM(u8 com)
{
	u8 i, k;
	LCD_CS_LOW; // _CS1 = 0;
	DELAY(2);
	k = 0x80;
	for (i = 0; i < 3; i++) // Write CMD code:100
	{
		LCD_WR_LOW; // _WR = 0;
		DELAY(2);
		if (k & 0x80) // _DAT = k & 0x80;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		DELAY(2);
		LCD_WR_HIGH; // _WR = 1;
		DELAY(2);
		k <<= 1;
	}
	k = com;
	for (i = 0; i < 9; i++) // Write CMD
	{
		LCD_WR_LOW; // _WR = 0;
		DELAY(2);
		if (k & 0x80) // _DAT = k & 0x80;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		DELAY(2);
		LCD_WR_HIGH; // _WR = 1;
		DELAY(2);
		k <<= 1;
	}
	LCD_CS_HIGH; // _CS1 = 1;
	DELAY(2);
}

void WRITE_DAT(u8 addr, u8 dat)
{
	u8 i, k;
	LCD_CS_LOW; // _CS1 = 0;
	DELAY(2);

	k = 0xa0;
	for (i = 0; i < 3; i++) // Write CMD code:101
	{
		LCD_WR_LOW; // _WR = 0;
		DELAY(2);
		if (k & 0x80) // _DAT = k & 0x80;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		DELAY(2);
		LCD_WR_HIGH; // _WR = 1;
		DELAY(2);
		k <<= 1;
	}
	k = addr << 2;			// 低六位
	for (i = 0; i < 6; i++) // Write RAM address: xxA5~A0
	{
		LCD_WR_LOW; // _WR = 0;
		DELAY(2);
		if (k & 0x80) // _DAT = k & 0x80;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		DELAY(2);
		LCD_WR_HIGH; // _WR = 1;
		DELAY(2);
		k <<= 1;
	}
	k = dat;				// 低四位
	for (i = 0; i < 4; i++) // Write Display data :xxxxC3~C0
	{
		LCD_WR_LOW; // _WR = 0;
		DELAY(2);
		if (k & 0x01) // _DAT = k & 0x01;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		DELAY(2);
		LCD_WR_HIGH; // _WR = 1;
		DELAY(2);
		k >>= 1;
	}
	LCD_CS_HIGH; // _CS1 = 1;
	DELAY(2);
}

/*---------写全屏数据------------*/
void DIS(u8 dat)
{
	u8 i;
	for (i = 0; i < 12; i++)
	{
		WRITE_DAT(i, dat);
		DELAY(200);
	}
}

/*----------初始化LCD------------*/
void SETUP_LCD(void)
{
	WRITE_COM(0x01); // Enable system
	WRITE_COM(0x18); // CLOCK SOURCE IS 256kRC ON CHIP
	WRITE_COM(0x29); // BIAS=1/3 DUTY=1/4(29),1/2B,1/2D(20),1/2B 1/4D(28)
	// WRITE_COM(0x02); // Display off
	WRITE_COM(0x03); // Display on
	// WRITE_COM(0x0E); // Clear the contents of WDT stage
	// WRITE_COM(0x07); // Enable WDT time-out flag output
	// WRITE_COM(0xA7); // Time base/WDT clock output:128Hz
	// WRITE_COM(0x88); // Enable IRQ output，背光关闭
	// // DELAY(2000);			 //延时显示停滞时间
	// WRITE_COM(0x80); // Disable IRQ output, 背光打开
	DIS(0x00);
}

void ShowRSOC(u8 n) // 电量显示
{
	if (n > 99)
	{
		WRITE_DAT(7, data_table2[0] | 1);
		WRITE_DAT(8, data_table1[0]);
		WRITE_DAT(9, data_table2[0] | 1);
		WRITE_DAT(10, data_table1[0]);
		WRITE_DAT(6, 0b1111);
		WRITE_DAT(11, 0b1111);
		return;
	}
	u8 t10, t1;
	t1 = n % 10;
	t10 = (n - t1) / 10;
	WRITE_DAT(9, data_table2[t10]);
	WRITE_DAT(10, data_table1[t10]);
	WRITE_DAT(7, data_table2[t1] | 1);
	WRITE_DAT(8, data_table1[t1]);
	t1 = (n << 3) / 100;
	if (t1 < 4)
	{
		WRITE_DAT(11, (1 << (t1 + 1)) - 1);
	}
	else
	{
		WRITE_DAT(11, 0b1111);
		WRITE_DAT(6, ((1 << (t1 - 3)) - 1) << (7 - t1));
	}
}

void ShowErrorAndCharges(u8 n) // 错误显示, 大于9清空
{
	if (n > 9)
	{
		WRITE_DAT(2, 0);
		WRITE_DAT(3, Global.ChargingFlag << 3);
		WRITE_DAT(4, 0);
		WRITE_DAT(5, Global.ReChargingFlag << 3);
		return;
	}
	WRITE_DAT(2, 0b1111);
	WRITE_DAT(3, 0b10 | (Global.ChargingFlag << 3));
	WRITE_DAT(4, data_table1[n]);
	WRITE_DAT(5, (data_table2[n] >> 1) | (Global.ReChargingFlag << 3));
}

void ShowNoNum(u8 n) // 显示机器序号，大于9清空
{
	if (n > 9)
	{
		WRITE_DAT(0, 0);
		WRITE_DAT(1, 0);
		return;
	}
	WRITE_DAT(0, data_table1[n]);
	WRITE_DAT(1, (data_table2[n] >> 1) | 0b1000);
}

void mainDisplay(void)
{
	u8 t; //
	SETUP_LCD(); // 初始化LCD
	while (1)
	{
		DIS(0x00); // 清显示
		for (t = 0; t < 11; t++)
		{
			ShowNoNum(t);
			DELAY(100000);
		}
		for (t = 0; t < 11; t++)
		{
			ShowErrorAndCharges(t);
			DELAY(100000);
		}
		for (t = 0; t < 101; t++)
		{
			ShowRSOC(t);
			DELAY(100000);
		}
	}
}
