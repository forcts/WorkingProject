// HHR88128-1049测试程序  AT89S52	12M
// LIQIAN
// 2023-3-31
// 测试程序    修改中
#include <LCDCtrol.h>

typedef unsigned char uchar;

// 依次显示出        	0    1    2    3    4    5    6    7    8

uchar data_table1[] = {0x0F, 0x00, 0x0B, 0x09, 0x04, 0x0D, 0x0F, 0x08, 0x0F, 0x0F}; // AFED数码8421
uchar data_table2[] = {0x0A, 0x0A, 0x0C, 0x0E, 0x0E, 0x06, 0x06, 0x0A, 0x0E, 0x0F}; // BGC*数码8421

//*******************延时子程序*******************//
void DELAY(long int t)
{
	int i, j;
	for (i = 0; i < t; i++)
		for (j = 0; j < 10; j++)
			;
}

/*-------写指令U1-----------*/
void WRITE_COM(uchar com)
{
	uchar i, k;
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
}
/*--------写数据U1-----------*/
void WRITE_DAT(uchar addr, uchar dat)
{
	uchar i, k;
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
}

/*---------写全屏数据------------*/
void DIS(uchar dat)
{
	uchar i;
	for (i = 0; i < 32; i++)
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
/*  ----------------- 8字显示 ------------- */
void Dsplay1(char c)
{
	uchar i;
	for (i = 0; i < 6;) //(U1)
	{
		WRITE_DAT(i, data_table1[c]);
		i++;
		WRITE_DAT(i, data_table2[c] >> 1);
		i++;
	}
	for (i = 7; i < 11;) //(U1)
	{
		WRITE_DAT(i, data_table2[c]);
		i++;
		WRITE_DAT(i, data_table1[c]);
		i++;
	}

	LCD_BL_TOGGLE;
}

void mainDisplay(void)
{

	uchar ii;	 //
	SETUP_LCD(); // 初始化LCD
	while (1)
	{
		LCD_BL_TOGGLE;
		DIS(0xff);	   // 全显
		DELAY(900000); // 延时显示停滞时间
		DIS(0x00);	   // 清显示
		DELAY(150000);
		for (ii = 0; ii < 10; ii++)
		{
			Dsplay1(ii); // 显示8字段
			LCD_BL_TOGGLE;
			DELAY(900000);
		}
		DIS(0xff); // 全显
		DELAY(600000);
		DIS(0x00); // 清显示
	}
}
