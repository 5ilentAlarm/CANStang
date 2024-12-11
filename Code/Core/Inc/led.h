#ifndef LED_H_
#define LED_H_

#include "main.h"
#include <stdbool.h>

#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define MAX_LED 1
#define USE_BRIGHTNESS 0
#define PI 3.14159265

extern bool datasentflag;

void init_user_led(void); // Initializes the USER LED pins
void blink_user_led(uint32_t delay); // Full blink routine, just to put in the main while loop
void Set_LED (int LEDnum, int Red, int Green, int Blue);
void WS2812_Send (void);
#endif