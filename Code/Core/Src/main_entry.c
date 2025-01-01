#include "main.h"
#include "fdcan.h"
#include "gpio.h"
#include "led.h"
#include "fdcan.h"
#include "usart.h"
// #include <stdio.h> for debug
#include "ssd1306.h"
#include <stdbool.h>


FDCAN_FilterTypeDef sFilterConfig = {
    .IdType = FDCAN_STANDARD_ID,
    .FilterIndex = 0,
    .FilterType = FDCAN_FILTER_MASK,
    .FilterConfig = FDCAN_FILTER_TO_RXFIFO0,
    .FilterID1 = 0x0,
    .FilterID2 = 0x0
};

/**
    Initialize peripherals, globals, etc.
*/
void main_init(void)
{
    /* UART peripheral settings */
    HAL_UART_Init(&huart2);

    /* CAN peripheral settings */
    FDCAN_Init_FilterConfig(sFilterConfig);
    FDCAN_Init_Notifications();
    HAL_FDCAN_Start(&hfdcan1);

    /* OLED */
    ssd1306_Init();

    /* Initialize LED to green */
    uint8_t led_index = 0u;
    LED_Set_Color(led_index, LED_COLOR_GREEN);
}

/** 
    Main runtime 'task'
*/
void main_entry_func(void)
{
    main_init();

    while(1)
    {
        LED_Determine_Color();
        ssd1306_Update_Rpm();
    }
}