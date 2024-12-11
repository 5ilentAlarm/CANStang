#include "main.h"
#include "fdcan.h"
#include "gpio.h"
#include "led.h"
#include "fdcan.h"
#include "stm32g4xx_hal.h"
#include "usart.h"
#include <stdio.h>
#include "stm32g4xx_hal_fdcan.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "i2c.h"
#include "dma.h"
#include <tim.h>
#include <stdbool.h>


FDCAN_FilterTypeDef sFilterConfig = {
    .IdType = FDCAN_STANDARD_ID,
    .FilterIndex = 0,
    .FilterType = FDCAN_FILTER_MASK,
    .FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
    .FilterID1 = 0x0,
    .FilterID2 = 0x0
};

static uint32_t start_time;

void main_entry_func(void)
{
    /* UART peripheral settings */
    HAL_UART_Init(&huart2);

    /* CAN peripheral settings */
    FDCAN_Init_FilterConfig(sFilterConfig);
    FDCAN_Init_Notifications();
    HAL_FDCAN_Start(&hfdcan1);

    /* OLED */
    ssd1306_Init();
    // uint8_t data[6];

    /* LED Settings */
    LED_Set_Color(0u, LED_COLOR_GREEN);

    while(1)
    {
        uint16_t Div_RPM = 0;
        Div_RPM = RPM % 1000;
        if(Div_RPM < 333)
        {
            LED_Set_Color(0u, LED_COLOR_GREEN);
        }
        else if (Div_RPM < 665)
        {
            LED_Set_Color(0u, LED_COLOR_YELLOW);
        }
        else
        {
            LED_Set_Color(0u, LED_COLOR_RED);
        }
        ssd1306_Update_Rpm();

        WS2812_Send();
    }
}