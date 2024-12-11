#include "led.h"
#include "gpio.h"
#include "stdbool.h"
#include <math.h> /* Please find a way to remove this */
#include "dma.h"
#include "tim.h"

bool datasentflag = 0;

uint8_t LED_Data[MAX_LED][4];
uint16_t pwmData[(24*MAX_LED)+50];

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

/**
    Uses the PWM/DMA controller to set the LED configuration
 */
void WS2812_Send (void)
{
	uint8_t index=0;
	uint32_t color;

	for (int i= 0; i < MAX_LED; i++)
	{
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));

		for (int i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				pwmData[index] = 80;  // 2/3 of 90
			}

			else 
            {
                pwmData[index] = 40;  // 1/3 of 90
            }
			index++;
		}

	}

	for (int i=0; i<50; i++)
	{
		pwmData[index] = 0;
		index++;
	}

	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t *)pwmData, index);
	while (!datasentflag){};
	datasentflag = 0;
}

void Set_LED (int LEDnum, int Red, int Green, int Blue)
{
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = Green;
	LED_Data[LEDnum][2] = Red;
	LED_Data[LEDnum][3] = Blue;
}