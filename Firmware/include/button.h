#pragma once
#ifndef _BUTTON_H_
#define _BUTTON_H

#include "Arduino.h"


typedef struct DEBOUNCE_BUTTON{

  uint8_t PIN;
  int lastCurrSwitch;
  uint8_t checkingSwitch;
  unsigned long timeStartToggled;  // the last time the output pin was toggled
  uint16_t timeOfHold;

} Button_t;


uint8_t Button_init(uint8_t pin, uint16_t timeOfHold, Button_t *button);
uint8_t ButtonCheckPress(Button_t *button);

#endif /*_BUTTON_H*/
