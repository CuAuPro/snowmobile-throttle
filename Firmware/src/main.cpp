#include "Arduino.h"

/**
  ******************************************************************************
  * @file           : Firmware.ino
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright.
  * All rights reserved.</center></h2>
  *
  * Firmware for snowmobile throttle cutoff when throttle
  * value is below treshold value, that is set by
  * calibration routine.      
  *                        
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <EEPROM.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "calib.h"
extern "C"
{
  #include "button.h"
  #include "settings.h"
}

/* USER CODE END Includes */


/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint16_t g_bottomThrottleLimit = 0;
Button_t ButtonCalibEntry;
Button_t ButtonCalibConfirm;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ErrorHandler(uint8_t errorCode);
/* USER CODE END PFP */

void setup()
{
  pinMode(CALIB_BTN_PIN, INPUT);  
  pinMode(THROTTLE_COND_PIN, OUTPUT);
  pinMode(LED_STATUS_PIN, OUTPUT);

  //Serial.begin(9600);
  if (EEPROM.read(NEW_MCU_FLG_ADDR)) // if value at address NEW_MCU_FLG_ADDR is not 0x00
  {
    EEPROM.write(NEW_MCU_FLG_ADDR, 0);
    g_bottomThrottleLimit = THROTTLE_DEFAULT_BOTTOM_LIMIT;
    EEPROM.put(THROTTLE_CALIB_VAL_ADDR, g_bottomThrottleLimit);
    ////Serial.write("First time MCU run. Var g_bottomThrottleLimit = ");
    ////Serial.println(g_bottomThrottleLimit);
  }
  else
  {
    EEPROM.get(THROTTLE_CALIB_VAL_ADDR, g_bottomThrottleLimit);
    //Serial.write("Var g_bottomThrottleLimit = ");
    //Serial.println(g_bottomThrottleLimit);
  }
  
  
  uint8_t init_status;
  init_status = Button_init(CALIB_BTN_PIN, BUTTON_CALIB_ENTRY_TIME, &ButtonCalibEntry);
  if (init_status < 0) 
  {
    ErrorHandler(ERROR_INITIALIZATION);
  }
  init_status = Button_init(CALIB_BTN_PIN, BUTTON_CALIB_CONFIRM_TIME, &ButtonCalibConfirm);
  if (init_status < 0) 
  {
    ErrorHandler(ERROR_INITIALIZATION);
  }
}


void loop()
{
  static uint8_t throttleCutoffFlg = 0;
  static uint8_t sigLedStatus = 0;

  uint16_t rawThrottle = analogRead(THROTTLE_PEDAL_PIN);

  if (rawThrottle <= g_bottomThrottleLimit)
  {
    if (throttleCutoffFlg == 1)
    {
      //Serial.write("Cut off throttle.\n");
      throttleCutoffFlg = 0;
      digitalWrite(THROTTLE_COND_PIN, throttleCutoffFlg);
    } 
  }
  if (rawThrottle > g_bottomThrottleLimit+THROTTLE_RAW_HYS)
  {
    if (throttleCutoffFlg == 0)
    {
      //Serial.write("Give back throttle.\n");
      throttleCutoffFlg = 1;
      digitalWrite(THROTTLE_COND_PIN, throttleCutoffFlg);
    } 
  }
  
  /*CHECK FOR CALIB REQUEST=================================*/
  uint8_t calibMode = ButtonCheckPress(&ButtonCalibEntry);
  if (calibMode)
  {
    //Serial.write("Stepping into calibration.\n");
    uint8_t calib_status = calibrateThrottle(THROTTLE_PEDAL_PIN, &ButtonCalibConfirm);
    if (calib_status == 1)
    {
      EEPROM.get(THROTTLE_CALIB_VAL_ADDR, g_bottomThrottleLimit);
      for (int i = 0;i<CALIB_CONFIRM_LED_BLINK_TIMES;i++)
      {
        sigLedStatus = !sigLedStatus;
        digitalWrite(LED_STATUS_PIN, sigLedStatus);
        delay(CALIB_CONFIRM_LED_TIME); //Here is intentionally delay()!
      }
      //Serial.write("Calibration completed.\n");
    }
    else
    {
      ErrorHandler(ERROR_CODE_CALIBRATION);
    }
  }
  /*END CHECK FOR CALIB REQUEST=================================*/
}


void ErrorHandler(uint8_t errorCode)
{
  uint8_t sigLedStatus = 0;
  while(1)
  {
    sigLedStatus = !sigLedStatus;
    digitalWrite(LED_STATUS_PIN, sigLedStatus);
    delay(errorCode);
  }
}