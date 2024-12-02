#include "led.h"
#include "gpio.h"

/**
 * Initialize the user LED on the nucleo STM32G431RB, located on pin PA5
 */
void init_user_led(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // RCC Clock already enabled for port A
    GPIO_InitStruct.Pin = LD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

/**
 * Blinks the LED 
 */
void blink_user_led(uint32_t delay)
{
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);   // LED ON
    HAL_Delay(delay);
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); // LED OFF
    HAL_Delay(delay);
}