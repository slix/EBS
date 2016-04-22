#ifndef EBS_EBS_H
#define EBS_EBS_H

#include "Gyroscope.h"
#include "FeedbackServo.h"
#include "Led.h"

class Ebs {
  public:
    // Initialize system and variables, but don't kick off any interrupts or loops yet
    Ebs();

    // Start all interrupts and timers that run independent of the main loop
    void start();

    // Run main loop
    void run();

  private:
    Gyro gyro;
    FeedbackServo feedback_servo;

    Led battery_led;
    Led mode_led;
    Led calibration_led;
};


#endif //EBS_H
