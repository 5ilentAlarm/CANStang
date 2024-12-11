#include "led.h"
#include "gpio.h"
#include "stdbool.h"
#include <math.h> /* Please find a way to remove this */
#include "dma.h"
#include "tim.h"
 

bool datasentflag = 0;

static uint8_t LED_Data[MAX_LED][4];
static uint16_t pwmData[(24 * MAX_LED) + 50];

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
			if (color & (1<<i))
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

/**
    Shift light only uses three colors, if more colors are wanted then they can be set here. 
 */
void LED_Set_Color(uint8_t Led_Index, Led_Color_T Color)
{
    uint8_t R, G, B;
    switch(Color)
    {
        case(LED_COLOR_GREEN):
            R = 0u;
            G = 150;
            B = 0u;
            break;
        case(LED_COLOR_RED):
            R = 150;
            G = 0u;
            B = 0u;
            break;
        case(LED_COLOR_YELLOW):
            R = 150u;
            G = 150u;
            B = 0u;
            break;
        default:
            R = 0;
            G = 0;
            B = 0;
    }
    LED_Data[Led_Index][0] = Led_Index;
    LED_Data[Led_Index][1] = G;
	LED_Data[Led_Index][2] = R;
	LED_Data[Led_Index][3] = B;
}