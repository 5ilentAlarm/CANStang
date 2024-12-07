#include "main.h"
#include "fdcan.h"
#include "gpio.h"
#include "led.h"
#include "fdcan.h"
#include "stm32g4xx_hal.h"
#include "usart.h"
#include <stdio.h>
#include "stm32g4xx_hal_fdcan.h"

static uint8_t msgdata[8];
static uint16_t RPM;

void main_entry_func(void)
{
    init_user_led();
    HAL_UART_Init(&huart2);

    FDCAN_FilterTypeDef sFilterConfig = {0};
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0x0;
    sFilterConfig.FilterID2 = 0x0;
    if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_FDCAN_Start(&hfdcan1);
    while(1)
    {
        // blink_user_led(1000u);
        // printf("Hi\r\n");
        if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &hrxcan1, &msgdata) == HAL_OK)
        {
            printf("Received CAN Message - ID: 0x%02x\r\n", hrxcan1.Identifier);
            for(uint8_t i = 0; i < hrxcan1.DataLength; i++)
            {
                // printf("%d ", msgdata[i]);
            }
            RPM = (msgdata[0] << 8) | msgdata[1];
            printf("\r\n");
            printf("RPM: %d\r\n", RPM);
        }
    }
}