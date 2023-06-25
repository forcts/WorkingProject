#include "n32g430_gpio.h"

#define LCD_SCK_PIN             GPIO_PIN_9 // was 6
#define LCD_SCK_SET_H           GPIO_Pins_Set(GPIOA,LCD_SCK_PIN)
#define LCD_SCK_SET_L           GPIO_Pins_Reset(GPIOA,LCD_SCK_PIN)
#define LCD_SCK_GET             GPIO_Input_Data_Get(GPIOA,LCD_SCK_PIN)

#define LCD_SDA_PIN             GPIO_PIN_10 // was 7
#define LCD_SDA_SET_H           GPIO_Pins_Set(GPIOA,LCD_SDA_PIN)
#define LCD_SDA_SET_L           GPIO_Pins_Reset(GPIOA,LCD_SDA_PIN)
#define LCD_SDA_GET             GPIO_Input_Data_Get(GPIOA,LCD_SDA_PIN)

#define LCD_RST_PIN             GPIO_PIN_1 // was 8
#define LCD_RST_SET_H           GPIO_Pins_Set(GPIOA,LCD_RST_PIN)
#define LCD_RST_SET_L           GPIO_Pins_Reset(GPIOA,LCD_RST_PIN)

#define LCD_BL_PIN              GPIO_PIN_7 // was 9
#define LCD_BL_SET_H            GPIO_Pins_Set(GPIOA,LCD_BL_PIN)
#define LCD_BL_SET_L            GPIO_Pins_Reset(GPIOA,LCD_BL_PIN)
#define LCD_BL_SIGNAl           GPIO_Input_Data_Get(GPIOA,LCD_BL_PIN)
#define LCD_BL_TOGGLE           GPIO_Pin_Toggle(GPIOA, LCD_BL_PIN)

#define mSOC_PERCENTAGE_UINT(_x) SOC_PERCENTAGE_UINT_##_x // S8-> s7
#define LCD_CS_HIGH LCD_RST_SET_H
#define LCD_CS_LOW LCD_RST_SET_L
#define mLCD_CS(_x) LCD_CS_##_x
#define LCD_WR_HIGH LCD_SCK_SET_H
#define LCD_WR_LOW LCD_SCK_SET_L
#define mLCD_WR(_x) LCD_WR_##_x
#define LCD_DATA_HIGH LCD_SDA_SET_H
#define LCD_DATA_LOW LCD_SDA_SET_L
#define mLCD_DATA(_x) LCD_DATA_##_x
#define LCD_BL_ON LCD_BL_SET_H
#define LCD_BL_OFF LCD_BL_SET_L
#define mLCD_BL(_x) LCD_BL_##_x

void lcd_init(void);
void display_all(void);
void mainDisplay(void);
