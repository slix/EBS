#include "FeedbackServo.h"

#include <Arduino.h>

FeedbackServo::FeedbackServo(int attach_pin, int feedback_pin_) {
  // Iniitalize writable servo
  write_servo.attach(attach_pin);

  // Put servo feedback in analog
  pinMode(feedback_pin_, INPUT);
  feedback_pin = feedback_pin_;
}

void FeedbackServo::set_angle(int angle) {
  write_servo.write(angle);
}

int FeedbackServo::get_feedback_angle() {
  // TODO: calibrate this to servo if possible
  int raw = analogRead(feedback_pin);
  return map(raw, 106, 461, 0, 180);
}