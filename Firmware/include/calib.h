#pragma once
#ifndef _CALIB_H_
#define _CALIB_H

#include "Arduino.h"
#include "EEPROM.h"

extern "C"
{
  #include "button.h"
  #include "settings.h"
}

uint8_t calibrateThrottle(int throttlePin, Button_t *button);

#endif /*_CALIB_H_*/