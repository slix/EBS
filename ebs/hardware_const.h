#ifndef EBS_HARDWARE_CONST_H
#define EBS_HARDWARE_CONST_H

namespace HardwareConst {
  // value = counter-clockwise; // clockwise 
  const int SERVO_ATTACH_PIN = 9;         // 2    // 8
  const int SERVO_FEEDBACK_PIN = A0;      // A7   /A7

  const int BATTERY_LED_PIN = 3;          // 7
  const int MODE_LED_PIN = 1;             // 9
  const int CALIBRATION_LED_PIN = 10;     // 0

  // These don't get used due to the complexity of bitshifting for interrupts
  const int DOWN_BUTTON_PIN = 0;          // 10
  const int UP_BUTTON_PIN = 2;            // 8
  const int CALIBRATION_BUTTON_PIN = 9;   // 1
}

#endif //EBS_HARDWARE_CONST_H
