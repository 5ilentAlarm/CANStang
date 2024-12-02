#ifndef LED_H_
#define LED_H_

#include "main.h"

#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

void init_user_led(void); // Initializes the USER LED pins

void blink_user_led(uint32_t delay); // Full blink routine, just to put in the main while loop
#endif