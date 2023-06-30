#include "n32g430_rcc.h"
#include "n32g430_flash.h"
#include "n32g430_tim.h"
#include "LCDCtrol.h"

ErrorStatus HSEStartUpStatus;
enum
{
    SYSCLK_PLLSRC_HSI,
    SYSCLK_PLLSRC_HSE,
};
void SetSysClockToPLL(uint32_t freq, uint8_t src)
{
    uint32_t pllsrc = (src == SYSCLK_PLLSRC_HSI ? RCC_PLL_SRC_HSI_DIV2 : RCC_PLL_SRC_HSE_DIV2);
    uint32_t pllmul;
    uint32_t latency;
    uint32_t pclk1div, pclk2div;

    if (HSE_VALUE != 8000000)
    {
        /* HSE_VALUE == 8000000 is needed in this project! */
        while (1)
            ;
    }

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration
     * -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_Reset(); // was RCC_DeInit();

    if (src == SYSCLK_PLLSRC_HSE)
    {
        /* Enable HSE */
        RCC_HSE_Config(RCC_HSE_ENABLE);

        /* Wait till HSE is ready */
        HSEStartUpStatus = RCC_HSE_Stable_Wait();

        if (HSEStartUpStatus != SUCCESS)
        {
            /* If HSE fails to start-up, the application will have wrong clock
               configuration. User can add here some code to deal with this
               error */

            /* Go to infinite loop */
            while (1)
                ;
        }
    }

    switch (freq)
    {
    case 24000000:
        latency = FLASH_LATENCY_0;
        pllmul = RCC_PLL_MUL_6;
        pclk1div = RCC_HCLK_DIV1;
        pclk2div = RCC_HCLK_DIV1;
        break;
    case 36000000:
        latency = FLASH_LATENCY_1;
        pllmul = RCC_PLL_MUL_9;
        pclk1div = RCC_HCLK_DIV1;
        pclk2div = RCC_HCLK_DIV1;
        break;
    case 48000000:
        latency = FLASH_LATENCY_1;
        pllmul = RCC_PLL_MUL_12;
        pclk1div = RCC_HCLK_DIV2;
        pclk2div = RCC_HCLK_DIV1;
        break;
    case 56000000:
        latency = FLASH_LATENCY_1;
        pllmul = RCC_PLL_MUL_14;
        pclk1div = RCC_HCLK_DIV2;
        pclk2div = RCC_HCLK_DIV1;
        break;
    case 72000000:
        latency = FLASH_LATENCY_2;
        pllmul = RCC_PLL_MUL_18;
        pclk1div = RCC_HCLK_DIV2;
        pclk2div = RCC_HCLK_DIV1;
        break;
    case 96000000:
        latency = FLASH_LATENCY_2;
        pllmul = RCC_PLL_MUL_24;
        pclk1div = RCC_HCLK_DIV4;
        pclk2div = RCC_HCLK_DIV2;
        break;
    case 128000000:
        latency = FLASH_LATENCY_3;
        pllmul = RCC_PLL_MUL_32;
        pclk1div = RCC_HCLK_DIV4;
        pclk2div = RCC_HCLK_DIV2;
        break;
    default:
        while (1)
            ;
    }

    FLASH_Latency_Set(latency);

    /* HCLK = SYSCLK */
    RCC_Hclk_Config(RCC_SYSCLK_DIV1);

    /* PCLK2 = HCLK */
    RCC_Pclk2_Config(pclk2div);

    /* PCLK1 = HCLK */
    RCC_Pclk1_Config(pclk1div);

    RCC_PLL_Config(pllsrc, pllmul);

    /* Enable PLL */
    RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (RCC_Flag_Status_Get(RCC_FLAG_PLLRD) == RESET)
        ;

    /* Select PLL as system clock source */
    RCC_Sysclk_Config(RCC_SYSCLK_SRC_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_Sysclk_Source_Get() != 0x08)
        ;
}

void CAN_GPIO_Config(void)
{
    GPIO_InitType GPIO_InitStructure;
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);
    GPIO_InitStructure.Pin = GPIO_PIN_4; // RX
    GPIO_InitStructure.GPIO_Mode = GPIO_PULL_UP; // was GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_CAN;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_5; // TX
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
}

void CAN_Filter_Init(void)
{
    CAN_FilterInitType CAN_FilterInitStructure;
    CAN_FilterInitStructure.Filter_Num = 0;
    CAN_FilterInitStructure.Filter_Mode = 0;
    CAN_FilterInitStructure.Filter_Scale = 0;
    CAN_FilterInitStructure.Filter_HighId = 0;
    CAN_FilterInitStructure.Filter_LowId = 0;
    CAN_FilterInitStructure.FilterMask_HighId = 0;
    CAN_FilterInitStructure.FilterMask_LowId = 0;
    CAN_FilterInitStructure.Filter_FIFOAssignment = 0;
    CAN_FilterInitStructure.Filter_Act = ENABLE;
    CAN_Filter_Initializes(&CAN_FilterInitStructure);
}

void CAN_Config(void)
{
    CAN_InitType CAN_InitStructure;
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_CAN);
    CAN_Reset(CAN);
    CAN_Structure_Initializes(&CAN_InitStructure);
    CAN_InitStructure.TTCM = DISABLE;
    CAN_InitStructure.ABOM = DISABLE;
    CAN_InitStructure.AWKUM = DISABLE;
    CAN_InitStructure.NART = ENABLE;
    CAN_InitStructure.RFLM = DISABLE;
    CAN_InitStructure.TXFP = ENABLE;
    CAN_InitStructure.OperatingMode = CAN_NORMAL_MODE;
    CAN_InitStructure.RSJW = CAN_RSJW_1TQ;
    CAN_InitStructure.TBS1 = CAN_TBS1_15TQ;
    CAN_InitStructure.TBS2 = CAN_TBS1_8TQ;
    CAN_InitStructure.BaudRatePrescaler = 4;
    CAN_Initializes(CAN, &CAN_InitStructure);
    CAN_Filter_Init();
}

void InitCan(void)
{
    CAN_GPIO_Config();
    CAN_Config();
}

void InitLCD(void)
{
    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);
    GPIO_InitType GPIO_InitStructure;
    GPIO_Structure_Initialize(&GPIO_InitStructure); // very important
    GPIO_InitStructure.Pin = LCD_BL_PIN | LCD_RST_PIN | LCD_SCK_PIN | LCD_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);

    mainDisplay();
    /*
    lcd_init();
    display_all();
    */
}

void InitUart(void)
{
    NVIC_InitType NVIC_InitStructure; // ÷–∂œ≈‰÷√
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_SUB_PRIORITY_2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Initializes(&NVIC_InitStructure);

    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOA);
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_USART2);

    GPIO_InitType GPIO_InitStructure; // GPIO ≈‰÷√
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_2; // TX
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_USART2;
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_3; // RX
    GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);

    USART_InitType USART_InitStructure; // Õ®—∂≈‰÷√
    USART_Reset(USART2);
    USART_InitStructure.BaudRate = 9600;
    USART_InitStructure.WordLength = USART_WL_8B;
    USART_InitStructure.StopBits = USART_STPB_1;
    USART_InitStructure.Parity = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode = USART_MODE_RX | USART_MODE_TX;
    USART_Initializes(USART2, &USART_InitStructure);
    USART_Interrput_Enable(USART2, USART_INT_RXDNE);
    USART_Enable(USART2);

    RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_GPIOB); //  πƒ‹485–æ∆¨
    GPIO_Structure_Initialize(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
}

static void User_TIMx_Init(void)
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    TIM_Base_Struct_Initialize(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Period = 0xFFFF;
    TIM_TimeBaseStructure.Prescaler = 0;
    TIM_TimeBaseStructure.ClkDiv = 0;
    TIM_TimeBaseStructure.CntMode = TIM_CNT_MODE_DOWN;
    RCC_APB1_Peripheral_Clock_Enable(RCC_APB1_PERIPH_TIM3);
    TIM_Base_Initialize(TIM3, &TIM_TimeBaseStructure);
    TIM_On(TIM3);
}

void InitControl(void)
{
    SetSysClockToPLL(96000000, SYSCLK_PLLSRC_HSI);
    User_TIMx_Init();
    InitCan();
    InitLCD();
    InitUart();
}
