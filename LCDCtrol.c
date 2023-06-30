#include "main.h"

void shortdelay(u16 k)
{
    for (u16 i = 0; i < k; i++)
        ;
}
void iic_send(unsigned char dat)
{
    unsigned char i;

    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
            LCD_DATA_HIGH; // sda = 1;
        else
            LCD_DATA_LOW; // sda = 0;
        LCD_WR_HIGH;      // scl = 1;
        shortdelay(2);
        LCD_WR_LOW; // scl = 0;
        shortdelay(2);
        dat = dat << 1;
    }
    LCD_DATA_HIGH; // sda = 1;
    LCD_WR_HIGH;   // scl = 1; //---signal A-----------//
    shortdelay(1);
    LCD_WR_LOW; // scl = 0;
    shortdelay(2);
}
/*============================
  I2c start condition
 SDA high->low while SCL=high
     ____________
  SCL            \_________
     _______
  SDA       \___________
============================*/
void iic_start() // 1send IIC start condition
{
    LCD_DATA_HIGH; // sda = 1;
    shortdelay(3);
    LCD_WR_HIGH; // scl = 1;
    shortdelay(3);
    LCD_DATA_LOW; // sda = 0;
    shortdelay(3);
    LCD_WR_LOW; // scl = 0;
    shortdelay(3);
}
/*============================
  I2c stop condition
  SDA low->high while SCL=high
                 _________
  SCL___________/
             _______
  SDA_______/
============================*/
void iic_stop()
{
    LCD_WR_HIGH; // scl = 1;
    shortdelay(3);
    LCD_DATA_LOW; // sda = 0;
    shortdelay(3);
    LCD_DATA_HIGH; // sda = 1;
    shortdelay(3);
}
/***************************************************************/
void delay(unsigned int z)
{
    unsigned char i, j, k;
    for (i = z; i > 0; i--)
    {
        for (j = 100; j > 0; j--)
        {
            for (k = 100; k > 0; k--)
                ;
        }
    }
}

void display_all(void)
{
    unsigned char i;
    iic_start();
    iic_send(0x7C); // slave address
    iic_send(0x80);
    iic_send(0x00); // Display Data Input Set
    for (i = 0; i < 33; i++)
    {
        iic_send(0xff);
    }
    iic_stop();
    LCD_BL_SET_H; // Light on
}

void display_clear(void)
{
    unsigned char i;
    iic_start();
    iic_send(0x7C); // slave address
    iic_send(0x80);
    iic_send(0x00); // Display Data Input Set
    for (i = 0; i < 33; i++)
    {
        iic_send(0x00);
    }
    iic_stop();
}

void display8(unsigned char table[])
{
    unsigned char i;
    iic_start();
    iic_send(0x7C); // slave address
    iic_send(0x80);
    iic_send(0x00); // Display Data Input Set
    for (i = 0; i < 33; i++)
    {
        iic_send(table[i]);
    }
    iic_stop();
}

void lcd_init()
{
    iic_start();
    iic_send(0x7C); // slave address
    iic_send(0x82); // ??????
    iic_send(0x00); // was 3 ??????
    iic_stop();

    iic_start();
    iic_send(0x7C); // slave address
    iic_send(0x84); // ?????????????? / ??????? / ??
    iic_send(0x03);
    iic_stop();

    iic_start();
    iic_send(0x7C); // slave address
    iic_send(0x86); // ??????????
    iic_send(0x00); // 0:80hz 1:160hz
    display_clear();
    iic_stop();

    iic_start();
    iic_send(0x7C); // slave address
    iic_send(0x8A); // ?????? (IVA) ????, ??????
    iic_send(0x00);
    iic_stop();
}

/*self made*/
void display(unsigned char address, unsigned char dat) // write on a address
{
    unsigned char i;
    iic_start();
    iic_send(0x7C); // slave address
    iic_send(0x80);
    iic_send(address); // set display address
    for (i = 0; i < 1; i++)
    {
        iic_send(dat);
    }
    iic_stop();
}
/*self made*/
