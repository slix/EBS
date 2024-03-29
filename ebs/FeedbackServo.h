// Servo class to control a servo's angle and get its feedback

#ifndef SERVO_H
#define SERVO_H

#include "internal_servo.h"

class FeedbackServo {
  public:
    // Construct FeedbackServo, angles written to attach_pin and feedback retrieved from feedback_pin
    FeedbackServo(int attach_pin, int feedback_pin);

    // Set servo to angle
    void set_angle(int angle);

    // Get reported angle from servo feedback
    int get_feedback_angle();

    // Get last written angle (for remembering previously set angle easily)
    int get_last_written_angle();

    // Call this often to update the servo
    void refresh();

  private:
    // Arduino Servo only supports writing
    Servo write_servo;

    int feedback_pin;

    int last_written_angle;
};

#endif
