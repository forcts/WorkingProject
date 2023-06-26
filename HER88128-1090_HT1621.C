// HHR88128-1049测试程序  AT89S52	12M
// LIQIAN
// 2023-3-31
// 测试程序    修改中
#include <LCDCtrol.h>
// #include <math.h>
// #include <intrins.h>
typedef unsigned char uchar;
typedef unsigned int uint;
// 依次显示出
// 依次显示出        	0    1    2    3    4    5    6    7    8

//  uchar code data_table1[]={0x03,0x00,0x02,0x00,0x01,0x01,0x03,0x00,0x03,0x0F};//**EF数码8421
//  uchar code data_table2[]={0x02,0x00,0x03,0x03,0x01,0x03,0x03,0x00,0x03,0x0F};//**DG数码8421
//  uchar code data_table3[]={0x03,0x03,0x01,0x03,0x03,0x02,0x02,0x03,0x03,0x0F};//**CB数码8421
//  uchar code data_table4[]={0x03,0x00,0x03,0x03,0x00,0x03,0x03,0x03,0x03,0x0F};//**AA数码8421

uchar data_table1[] = {0x0F, 0x00, 0x0B, 0x09, 0x04, 0x0D, 0x0F, 0x08, 0x0F, 0x0F}; // AFED数码8421
uchar data_table2[] = {0x0A, 0x0A, 0x0C, 0x0E, 0x0E, 0x06, 0x06, 0x0A, 0x0E, 0x0F}; // BGC*数码8421

// uchar code data_table1[] = {0x0D, 0x00, 0x0E, 0x0A, 0x03, 0x0B, 0x0F, 0x00, 0x0F, 0x0F}; // DEGF数码8421
// uchar code data_table2[] = {0x07, 0x06, 0x03, 0x07, 0x06, 0x05, 0x05, 0x07, 0x07, 0x0F}; //*CBA数码8421

// uchar code data_table5[] = {0x05, 0x00, 0x06, 0x02, 0x03, 0x03, 0x07, 0x00, 0x07, 0x0F}; //*EGF数码8421
// uchar code data_table6[] = {0x0F, 0x06, 0x0B, 0x0F, 0x06, 0x0D, 0x0D, 0x07, 0x0F, 0x0F}; // DCBA数码8421

// uchar code data_table1[] = {0x0F, 0x04, 0x0D, 0x0D, 0x0E, 0x0B, 0x0B, 0x05, 0x0F, 0x0F}; // GBFA数码8421
// uchar code data_table2[] = {0x07, 0x02, 0x05, 0x06, 0x02, 0x06, 0x07, 0x02, 0x07, 0x0F}; //*DCE数码8421

// uchar code data_table2[] = {0x0E, 0x06, 0x0C, 0x0E, 0x06, 0x0A, 0x0A, 0x0E, 0x0E, 0x0F}; // ABC*数码8421
// uchar code data_table1[] = {0x0B, 0x00, 0x07, 0x05, 0x0C, 0x0D, 0x0F, 0x00, 0x0F, 0x0F}; // FGED数码8421

// uchar code data_table1[] = {0x0A, 0x00, 0x03, 0x01, 0x09, 0x09, 0x0B, 0x00, 0x0B, 0x0F}; // F*EG数码8421
// uchar code data_table2[] = {0x0F, 0x03, 0x0D, 0x0F, 0x03, 0x0E, 0x0E, 0x0B, 0x0F, 0x0F}; // ADCB数码8421

// uchar code data_table2[] = {0x05, 0x05, 0x03, 0x07, 0x07, 0x06, 0x06, 0x05, 0x07, 0x0F}; //*CGB数码8421
// uchar code data_table1[] = {0x0F, 0x00, 0x0D, 0x09, 0x02, 0x0B, 0x0F, 0x01, 0x0F, 0x0F}; // DEFA数码8421

// uchar code data_table4[] = {0x05, 0x00, 0x03, 0x02, 0x06, 0x06, 0x07, 0x00, 0x07, 0x0F}; //*FGE数码8421
// uchar code data_table3[] = {0x0F, 0x06, 0x0D, 0x0F, 0x06, 0x0B, 0x0B, 0x0E, 0x0F, 0x0F}; // ABCD数码8421

// uchar code data_table3[] = {0x07, 0x00, 0x05, 0x01, 0x02, 0x03, 0x07, 0x01, 0x07, 0x0F}; //*EFA数码8421
// uchar code data_table4[] = {0x0D, 0x05, 0x0B, 0x0F, 0x07, 0x0E, 0x0E, 0x05, 0x0F, 0x0F}; // DCGB数码8421

// uchar code data_table1[] = {0x0A, 0x00, 0x06, 0x04, 0x0C, 0x0C, 0x0E, 0x00, 0x0E}; // FGE*数码(U1)(U2)
// uchar code data_table2[] = {0x0F, 0x06, 0x0D, 0x0F, 0x06, 0x0B, 0x0B, 0x0E, 0x0F}; // ABCD数码(U1)(U2)

// uchar code data_table3[]={0x08,0x0C,0x0E,0x0F,0x0F,0x0F,0x0F,0x00};//EC数码(U2)
// uchar code data_table4[]={0x00,0x00,0x00,0x00,0x02,0x0A,0x0E,0x00};//D*数码(U2)
// 依次显示出A    B    C    D    E    F    G    H    I    J    K    L    M    N
// uchar code data_table5[]={0x00,0x02,0x06,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E};//*BCD数码(U3)
// uchar code data_table6[]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x07,0x0F,0x0F,0x0F};//AJKL数码(U3)
// uchar code data_table7[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x07,0x0F};//HIMN数码(U3)
// uchar code data_table8[]={0x00,0x00,0x00,0x00,0x04,0x05,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07};//FGE*数码(U3)

/*
sbit _CS1 = P3 ^ 4; // U1

sbit _DAT = P1 ^ 0;
sbit _RD = P3 ^ 2;
sbit _WR = P3 ^ 1;

sbit LED1 = P3 ^ 7;
*/

void Display(void);
void Display1(void);
void Display2(void);
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

	k = 0x80;
	for (i = 0; i < 3; i++) // Write CMD code:100
	{
		LCD_WR_LOW;	  // _WR = 0;
		if (k & 0x80) // _DAT = k & 0x80;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		LCD_WR_HIGH; // _WR = 1;
		k <<= 1;
	}
	k = com;
	for (i = 0; i < 9; i++) // Write CMD
	{
		LCD_WR_LOW;	  // _WR = 0;
		if (k & 0x80) // _DAT = k & 0x80;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		LCD_WR_HIGH; // _WR = 1;
		k <<= 1;
	}
	LCD_CS_HIGH; // _CS1 = 1;
}
/*--------写数据U1-----------*/
void WRITE_DAT(uchar addr, uchar dat)
{
	uchar i, k;
	LCD_CS_LOW; // _CS1 = 0;

	k = 0xa0;
	for (i = 0; i < 3; i++) // Write CMD code:101
	{
		LCD_WR_LOW;	  // _WR = 0;
		if (k & 0x80) // _DAT = k & 0x80;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		LCD_WR_HIGH; // _WR = 1;
		k <<= 1;
	}
	k = addr << 2;			// 低六位
	for (i = 0; i < 6; i++) // Write RAM address: xxA5~A0
	{
		LCD_WR_LOW;	  // _WR = 0;
		if (k & 0x80) // _DAT = k & 0x80;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		LCD_WR_HIGH; // _WR = 1;
		k <<= 1;
	}
	k = dat;				// 低四位
	for (i = 0; i < 4; i++) // Write Display data :xxxxC3~C0
	{
		LCD_WR_LOW;	  // _WR = 0;
		if (k & 0x01) // _DAT = k & 0x01;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		LCD_WR_HIGH; // _WR = 1;
		k >>= 1;
	}
	LCD_CS_HIGH; // _CS1 = 1;
}

/*--------读数据-----------*/
/* not used
uchar Read_DAT(uchar addr)
{
	uchar i, k, dat;
	LCD_CS_LOW; // _CS1 = 0;
	k = 0xc0;
	for (i = 0; i < 3; i++) // Read CMD code:110
	{
		LCD_WR_LOW;	  // _WR = 0;
		if (k & 0x80) // _DAT = k & 0x80;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		LCD_WR_HIGH; // _WR = 1;
		k <<= 1;
	}
	k = addr << 2;			// 低六位
	for (i = 0; i < 6; i++) // Write RAM address: xxA5~A0
	{
		LCD_WR_LOW;	  // _WR = 0;
		if (k & 0x80) // _DAT = k & 0x80;
			LCD_DATA_HIGH;
		else
			LCD_DATA_LOW;
		LCD_WR_HIGH; // _WR = 1;
		k <<= 1;
	}
	dat = 0x00;
	LCD_DATA_HIGH;			// _DAT = 1;				//  先给数据线赋1；读低四位
	for (i = 0; i < 4; i++) // Read data :xxxxC0~C3
	{
		dat >>= 1;

		_RD = 0;
		_nop_();
		_nop_();

		if (_DAT == 1)
			dat = dat | 0x80;

		_RD = 1;
	}
	LCD_CS_HIGH; // _CS1 = 1;
	dat >>= 4;
	return dat;
}
*/
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
					 //	WRITE_COM(0x02); 			// Display off
	WRITE_COM(0x03); // Display on
					 /*
					   WRITE_COM(0x0E); 			// Clear the contents of WDT stage
						 WRITE_COM(0x07); 			// Enable WDT time-out flag output
					   WRITE_COM(0xA7); 			// Time base/WDT clock output:128Hz
						 WRITE_COM(0x88); 			// Enable IRQ output，背光关闭
						 //DELAY(2000);			 //延时显示停滞时间
						 WRITE_COM(0x80); 			// Disable IRQ output, 背光打开
					 */
					 //	DIS(0xff);		 			// Clear LCD
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

void segdsplay(uint t) // 笔段测试
{
	WRITE_DAT(8, 0x0F); // S9
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(10, 0x0F); // S5
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(0, 0x01); // S4
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(0, 0x03); // S3
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(0, 0x07); // S2
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(0, 0x0F); // S1
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(2, 0x08); // S8
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(2, 0x0C); // S7
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(2, 0x0E); // S6
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(1, 0x01); // S13
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(1, 0x03); // S12
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(1, 0x07); // S11
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(1, 0x0F); // S10
	LCD_BL_TOGGLE;
	DELAY(t);
	WRITE_DAT(2, 0x0F); // S14
	LCD_BL_TOGGLE;
	DELAY(t);

	DELAY(t);
}

void flash(uint tt)
{
	DELAY(tt);
	WRITE_COM(0x02); // Display off
	DELAY(tt);
	WRITE_COM(0x03); // Display on
	DELAY(tt);
	WRITE_COM(0x02); // Display off
	DELAY(tt);
	WRITE_COM(0x03); // Display on
}

// 旧屏驱动

/*
 *    LCD 模式写入
 *    入口:MODE :  0 ：COM(命令模式) /  1：DAT(数据模式)
 *    出口:void
*/
void write_mode(unsigned char MODE)    //写入模式,数据or命令
{
    mLCD_WR(LOW);
    DELAY(2);
    mLCD_DATA(HIGH);//SET_DATA;     //DATA=1
    DELAY(2);
    mLCD_WR(HIGH);//SET_WR;
    DELAY(2);
 
    mLCD_WR(LOW);//RST_WR;       //WR = 0;
    DELAY(2);
    mLCD_DATA(LOW);//RST_DATA;     //DATA = 0;
    DELAY(2);
    mLCD_WR(HIGH);//SET_WR;       //WR = 1;
    DELAY(2);
 
    mLCD_WR(LOW);//RST_WR;                                 
    DELAY(2);
 
    if (MODE)
    {
      mLCD_DATA(HIGH);//SET_DATA;  //DATA = 1;  写数据模式
    }
    else
    {
      mLCD_DATA(LOW);//RST_DATA; //DATA = 0;  写命令模式
    }
    DELAY(2);
    mLCD_WR(HIGH);//SET_WR;                                   
    DELAY(2);
}

//Write the high BitSize of the Data to the HT1621
void SendBit_HT1621(uint8_t Data, uint8_t BitSize)
{
    uint8_t i;
    for(i=0;i<BitSize;i++)
    {
        if(Data & 0x80)
        {
            mLCD_DATA(HIGH);
        }
        else
        {
            mLCD_DATA(LOW);
        }
		mLCD_WR(LOW);
		DELAY(2);
        mLCD_WR(HIGH);
        DELAY(2);
        Data<<=1;
    }
}

void HT1621_Init(void)
{
    mLCD_BL(ON);
    mLCD_CS(HIGH);
    mLCD_WR(HIGH);
    mLCD_DATA(HIGH);
	
	mLCD_CS(LOW);
    DELAY(2);
	
	write_mode(0);
	
	SendBit_HT1621(0x01, 9);//Write 9 bit cmd   0x01
	SendBit_HT1621(0x03, 9);//Write 9 bit cmd   0x03
	SendBit_HT1621(0x04, 9);
	SendBit_HT1621(0x05, 9);//Write 9 bit cmd  0x05	
	SendBit_HT1621(0x18, 9);//Write 9 bit cmd   0x18
	SendBit_HT1621(0x29, 9);//Write 9 bit cmd    0x29
	SendBit_HT1621(0x80, 9);	
	SendBit_HT1621(0xE3, 9);//Write 9 bit cmd   0x18

    DELAY(100);
}

// 旧屏驱动

void mainDisplay(void)
{

	uchar ii;	 //
	SETUP_LCD(); // 初始化LCD
	while (1)
	{
		LCD_BL_TOGGLE;

		DIS(0xff);	 // 全显
		DELAY(3000); // 延时显示停滞时间

		DIS(0x00); // 清显示
		DELAY(500);

		for (ii = 0; ii < 9; ii++)
		{
			Dsplay1(ii); // 显示8字段
			LCD_BL_TOGGLE;
			DELAY(3000);
		}

		DIS(0xff); // 全显
		DELAY(1900);
		DIS(0x00); // 清显示
				   //	segdsplay(1500);	 //段显示
				   //   DIS(0x00);				 //清显示
				   /*
						DELAY(2900);flash(1200);
						DELAY(3900);DIS(0x00);
						//读测试
						ii = 0x00;
						WRITE_DAT(22,0x01);
						DELAY(3900);//ODO亮
						ii = Read_DAT(22);
						DELAY(900);
						WRITE_DAT(23,ii);//TRIP亮
						DELAY(3900);
						DIS(0x00); */
	}
}
