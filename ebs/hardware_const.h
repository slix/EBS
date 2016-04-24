#ifndef EBS_HARDWARE_CONST_H
#define EBS_HARDWARE_CONST_H

namespace HardwareConst {
  const int SERVO_ATTACH_PIN = 9;
  const int SERVO_FEEDBACK_PIN = A0;

  const int BATTERY_LED_PIN = 7;
  const int MODE_LED_PIN = 9;
  const int CALIBRATION_LED_PIN = -1;

  // These don't get used due to the complexity of bitshifting for interrupts
  const int DOWN_BUTTON_PIN = 10;
  const int UP_BUTTON_PIN = 8;
  const int CALIBRATION_BUTTON_PIN = 1;
}

#endif //EBS_HARDWARE_CONST_H
