/*
  Built off of the Arduino_CAN example for the UNO R4
  This is meant to simulate RPM traffic coming from the CAN bus.
*/

#include <Arduino_CAN.h>

static uint32_t const CAN_ID = 0x109;

void setup()
{
  Serial.begin(115200);
  while (!Serial) { }

  if (!CAN.begin(CanBitRate::BR_500k))
  {
    Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }
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