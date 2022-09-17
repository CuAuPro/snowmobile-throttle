#pragma once
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "Arduino.h"

#define THROTTLE_PEDAL_PIN              A1
#define THROTTLE_COND_PIN               0
#define CALIB_BTN_PIN                   4
#define LED_STATUS_PIN                  3

#define NEW_MCU_FLG_ADDR                0

#define THROTTLE_RAW_HYS                16
#define THROTTLE_CALIB_VAL_ADDR         10
#define THROTTLE_DEFAULT_BOTTOM_LIMIT   16


#define BUTTON_CALIB_ENTRY_TIME         2000
#define BUTTON_CALIB_CONFIRM_TIME       1000

#define CALIB_MENU_TIME_LED             250
#define CALIB_CONFIRM_LED_TIME          100
#define CALIB_CONFIRM_LED_BLINK_TIMES   10

#define ERROR_CODE_CALIBRATION          100
#define ERROR_INITIALIZATION            200
#endif /*_SETTINGS_H_*/