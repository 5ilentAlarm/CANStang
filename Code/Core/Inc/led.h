#ifndef LED_H_
#define LED_H_

#include "main.h"
#include <stdbool.h>
#include <stdint.h>

#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define MAX_LED 1

extern bool Data_Sent_Flag;

/* LED color modes */
typedef enum
{
    LED_COLOR_GREEN,
    LED_COLOR_YELLOW,
    LED_COLOR_RED
} Led_Color_T;

void init_user_led(void); // Initializes the USER LED pins
void blink_user_led(uint32_t delay); // Full blink routine
void LED_Set_Color(uint8_t Led_Index, Led_Color_T Color); // Sets LED color
void WS2812_Transmit(void); // Translates RGB data into PWM to be sent using DMA
void LED_Determine_Color(void); // Uses RPM data to determine what color the LED should be at
#endif