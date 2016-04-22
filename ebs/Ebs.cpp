#include "Ebs.h"
#include "hardware_const.h"

Ebs::Ebs() :
  gyro(),
  feedback_servo(HardwareConst::SERVO_ATTACH_PIN, HardwareConst::SERVO_FEEDBACK_PIN),
  battery_led(HardwareConst::BATTERY_LED_PIN),
  mode_led(HardwareConst::MODE_LED_PIN),
  calibration_led(HardwareConst::CALIBRATION_LED_PIN)
{
}

void Ebs::start() {

}

void Ebs::run() {

}