/*
  Built off of the Arduino_CAN example for the UNO R4
  This is meant to simulate RPM traffic coming from the CAN bus.
*/

#include <Arduino_CAN.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

static uint32_t const CAN_ID = 0x109;

// Defines for OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SDA 14
#define SCL 15
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void setup()
{
  Serial.begin(115200);
  while (!Serial) { }

  if (!CAN.begin(CanBitRate::BR_500k))
  {
    Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }

  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
    Serial.println(F("Display1 SSD1306 allocation failed"));
  }

  testdrawchar();
}

static uint32_t msg_cnt = 0;
uint16_t RPM = 0;
void loop()
{
  /*
    Transmits a message on the CAN bus
    The receiving side can access data bytes 1 and 0 to get the RPM.
    Use the following Eq: 
      uint16_t RPM = (rxFrame.data[0] << 8) | (rxFrame.data[1]);
      Serial.printf("Data: %d\n", RPM);
  */
  uint8_t rpm_high_byte = (RPM >> 8) & 0xFF;
  uint8_t rpm_low_byte = RPM & 0xFF;
  uint8_t const msg_data[] = {rpm_high_byte,rpm_low_byte,1,2,3,4,5,6};
  // memcpy((void *)(msg_data + 4), &msg_cnt, sizeof(msg_cnt));
  CanMsg const msg(CanStandardId(CAN_ID), sizeof(msg_data), msg_data);

  if (int const rc = CAN.write(msg); rc < 0)
  {
    Serial.print  ("CAN.write(...) failed with error code ");
    Serial.println(rc);
  }
  else
  {
    Serial.print("Successful!\n");
  }

  RPM++;

  delay(100);
}
