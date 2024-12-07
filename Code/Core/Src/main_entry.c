#include "main.h"
#include "fdcan.h"
#include "gpio.h"
#include "led.h"
#include "fdcan.h"
#include "stm32g4xx_hal.h"
#include "usart.h"
#include <stdio.h>
#include "stm32g4xx_hal_fdcan.h"

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


    start_time = HAL_GetTick();
    while(1)
    {
        if((HAL_GetTick() - start_time) >= 500)
        {
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            start_time = HAL_GetTick();
            printf("RPM in main = %d\r\n", RPM);
        }
    }
}