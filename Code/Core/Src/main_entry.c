#include "main.h"
#include "fdcan.h"
#include "gpio.h"
#include "led.h"
#include "fdcan.h"
#include "stm32g4xx_hal.h"
#include "usart.h"
#include <stdio.h>

void main_entry_func(void)
{
    init_user_led();
    HAL_UART_Init(&huart2);
    HAL_FDCAN_Start(&hfdcan1);
    while(1)
    {
        blink_user_led(1000u);
        printf("Hi\r\n");
    }
}