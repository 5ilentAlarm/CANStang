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
    /* Intitialize pins for on-board LED */
    init_user_led();

    /* UART peripheral settings */
    HAL_UART_Init(&huart2);

    /* CAN peripheral settings */
    FDCAN_Init_FilterConfig(sFilterConfig);
    FDCAN_Init_Notifications();
    HAL_FDCAN_Start(&hfdcan1);

    /* OLED */
    ssd1306_Init();

    start_time = HAL_GetTick();
    uint8_t data[6];

    // ssd1306_DrawPixel(30, 30, White);
    while(1)
    {
        if((HAL_GetTick() - start_time) >= 500)
        {
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            start_time = HAL_GetTick();
            // printf("RPM in main = %d\r\n", RPM);
            
        }
        int x = (128 - (sizeof(data) * 16)) / 2;
        int y = (64 - 26) / 2; 
        ssd1306_SetCursor(x, y);

        sprintf(data,"%d",RPM);    

        ssd1306_Fill(Black);
        ssd1306_UpdateScreen();
        ssd1306_WriteString(data, Font_16x26, White);
        ssd1306_UpdateScreen();
        // Set_Brightness(0);
        Set_LED(0, 100, 100, 0);

        WS2812_Send();
    }
}