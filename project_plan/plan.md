# CANStang
<img src="images/mustang_fall_jpeg.jpg" alt="Car" width="400"/>

## Overview
A custom solution to intercepting the CAN bus on my 2015 Mustang GT. The two main features that I want out of this is 
- A shift light that can either go on the steering wheel / or 
at the back of my dash and act as a HUD. 
- A digital speedometer that will be on the HUD.
- (Optional) A gear indicator on the HUD as well.

## Hardware
### Microcontroller
Need to select a microcontroller capable of CAN, SPI, and I2C at the minimum. Everything else will be a plus. For the sake of cost, availibility, and popularity, an STM32 variant will do.
- STM32F103RB
- STM32L476RCT6
    - $5.50 per
    - 3x SPI, 1 CAN, 3x I2C
    - [Webpage](https://www.st.com/en/microcontrollers-microprocessors/stm32l476rc.html)
- STM32L432
    - $4.00+
    - 2x SPI, 2x I2C, 1x CAN
- STM32F446RE 
- STM32G431RB
    - $3.90
    - 3x I2C, 3x SPI, 1x CANFD
- Renesas RA4M1
- SAME51J20A
- LPC1800

There are probably a near infinite amount of options for a microcontroller, but I won't dig too deep. From the above, it'll be best to pick the one that 
1. Is cost-effective
2. Easier to implement i.e On-board memory
3. Has the best amount of resources for implementation. 
4. Comes in a development board.

#### Final decision
The STM32G431XX series chips is the one I'm going with. This is due to cost, since a lot of the other chips are >$5, and because it supports a clock speed up to 170Mhz, which makes things a little better for CAN sniffing.

[Nucleo Board](https://estore.st.com/en/nucleo-g431rb-cpn.html)

[Flashing tool for PCB](https://www.st.com/en/development-tools/stlink-v3minie.html)

##### Features
- 3x I2C
- 3x SPI
- CANFD
- 170MHz clock speed
- Has cheaper variants that are available (as of December 2024)
### LED Strip
It'll be wortwhile to just get a small LED strip, using WS2812b's or similar, that can be trimmed down if needed. 
These can then be put onto the final PCB as well. 

[WS2812B Strip](https://www.amazon.com/WS2812B-Individual-Addressable-144Pixels-Non-Waterproof/dp/B09PBGZMNS?)

### HUD Components
The plan is to mount an OLED screen onto the PCB, and place this towards the front of the dash
, under the windshield. A small reflective screen will be stuck to the windshield to reflect the values. 

 1. Reflective HUD screen for the Car windshield, can trim this down
    - [HUD Reflector](https://www.amazon.com/Xbes-Reflective-Windshield-Protective-5-9X5-1Inch/dp/B075DBXL9Z?)
 2. Decide between OLED or 7-Segment display for RPM / Speed
    - [OLED 128x32](https://www.amazon.com/MakerFocus-Display-SSD1306-3-3V-5V-Arduino/dp/B079BN2J8V?)

# Architecture
<img src="images/High-level-Architecture.png" alt="Car" width="600"/>


# Resources
[S550 Mustang GT CAN Id list](CAN_IDs.md)

[SSD1306 STM32 Library](https://github.com/afiskon/stm32-ssd1306)

[Another SSD1306 Library](https://github.com/4ilo/ssd1306-stm32HAL?tab=readme-ov-file)

