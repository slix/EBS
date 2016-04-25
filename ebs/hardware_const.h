#ifndef EBS_HARDWARE_CONST_H
#define EBS_HARDWARE_CONST_H

#include "system_const.h"

namespace HardwareConst {

#if USE_ATTINY_PINS
  const int SERVO_ATTACH_PIN = 8;
  const int SERVO_FEEDBACK_PIN = A7;

  const int BATTERY_LED_PIN = 7;
  const int MODE_LED_PIN = 9;
  const int CALIBRATION_LED_PIN = 10;

  // These don't get used due to the complexity of bitshifting for interrupts
  const int DOWN_BUTTON_PIN = 10;
  const int UP_BUTTON_PIN = 8;
  const int CALIBRATION_BUTTON_PIN = 1;

#else
  // Arduino Uno
  const int SERVO_ATTACH_PIN = 9;
  const int SERVO_FEEDBACK_PIN = A0;

  const int BATTERY_LED_PIN = 12;
  const int MODE_LED_PIN = 13;
  const int CALIBRATION_LED_PIN = 11;

  // These don't get used due to the complexity of bitshifting for interrupts
  const int DOWN_BUTTON_PIN = 7;
  const int UP_BUTTON_PIN = 6;
  const int CALIBRATION_BUTTON_PIN = 5;
#endif
}

#endif //EBS_HARDWARE_CONST_H
