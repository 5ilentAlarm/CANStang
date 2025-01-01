#include "led.h"
#include "gpio.h"
#include "stdbool.h"
#include "dma.h"
#include "tim.h"
#include "fdcan.h"
 
#define RGB_BYTES (3u)
#define RESET_PULSE_LENGTH (50u)

bool Data_Sent_Flag = 0;

/* WS2812B and similar Leds each take */
static uint8_t Led_Data[MAX_LED][RGB_BYTES];
static uint16_t Pwm_Data[(24 * MAX_LED) + RESET_PULSE_LENGTH];

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
void WS2812_Transmit(void)
{
	uint8_t index=0;
	uint32_t color;

	for (int i= 0; i < MAX_LED; i++)
	{
        /* Fill up the the 24-bits that we went to send out through PWM */
		color = ((Led_Data[i][0]<<16) | (Led_Data[i][1]<<8) | (Led_Data[i][2]));

        /* PWM logic based on timer configuration,  */
		for (int i = 23; i >= 0; i--)
		{
			if (color & (1<<i))
			{
				Pwm_Data[index] = 80;  // 2/3 of 90MHz, Logic ligh for bit in 'color'
			}
			else 
            {
                Pwm_Data[index] = 40;  // 1/3 of 90MHz, logic low for bit in 'color'
            }
			index++;
		}
	}

    /* Set the upper 50 half-words to 0, 50us low reset pulse as specified in the datasheet */
	for (int i = 0; i < RESET_PULSE_LENGTH; i++)
	{
		Pwm_Data[index] = 0;
		index++;
	}

    /* Begin a DMA transmission and wait for timer callback once PWM is finished */
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t *)Pwm_Data, index);
	while (!Data_Sent_Flag){};
	Data_Sent_Flag = 0;
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
    // Led_Data[Led_Index][0] = Led_Index;
    Led_Data[Led_Index][0] = G;
	Led_Data[Led_Index][1] = R;
	Led_Data[Led_Index][2] = B;
}

void LED_Determine_Color(void)
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
    WS2812_Transmit();
}