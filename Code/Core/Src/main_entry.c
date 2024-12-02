#include "main.h"
#include "fdcan.h"
#include "gpio.h"
#include "led.h"

void main_entry_func(void)
{
    init_user_led();

    while(1)
    {
        blink_user_led(1000u);
    }
}