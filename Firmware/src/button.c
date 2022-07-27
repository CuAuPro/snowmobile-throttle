#include "button.h"

uint8_t Button_init(uint8_t pin, uint16_t timeOfHold, Button_t *button)
{
  button->PIN = pin;
  button->lastCurrSwitch = 0;
  button->checkingSwitch = 0;
  button->timeStartToggled = 0;  // the last time the output pin was toggled
  button->timeOfHold = timeOfHold;
  return 1;
}

uint8_t ButtonCheckPress(Button_t *button)
{
  uint8_t ret = 0;

  int currSwitch = digitalRead(button->PIN);
  
  if( currSwitch != button->lastCurrSwitch )
  {
    if( currSwitch == 0 ) //if low now, the change was unpressed to pressed
    {
      button->timeStartToggled = millis();
      button->checkingSwitch = 1;
    }
    else //was a rising edge (button being released), cancel checking
    {
      button->checkingSwitch = 0;
    }
    button->lastCurrSwitch = currSwitch;
  }
  
  if( button->checkingSwitch )
  {
    if( (millis() - button->timeStartToggled ) >= button->timeOfHold )
    {
      button->checkingSwitch = 0;
      ret = 1;
    }
            
  }

  return ret;
}