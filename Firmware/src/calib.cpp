#include "calib.h"

uint8_t calibrateThrottle(int throttlePin, Button_t *button)
{
  uint8_t calibEnd = 0;
  unsigned long timeStartToggled = millis();
  uint8_t sigLedStatus = 0;

  while (calibEnd != 1)
  {
    if (millis() - timeStartToggled > CALIB_CONFIRM_LED_TIME)
    {
      sigLedStatus = !sigLedStatus;
      timeStartToggled = millis();
      digitalWrite(LED_STATUS_PIN, sigLedStatus);
    }
    uint8_t calibWrite = ButtonCheckPress(button);
    if (calibWrite)
    {
      uint16_t rawThrottle = analogRead(throttlePin);
      EEPROM.put(THROTTLE_CALIB_VAL_ADDR, rawThrottle);
      calibEnd = 1;
    }   
  }
  return 1;
}