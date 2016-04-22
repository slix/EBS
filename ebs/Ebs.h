#ifndef EBS_EBS_H
#define EBS_EBS_H

#include "Gyroscope.h"
#include "FeedbackServo.h"
#include "Led.h"

class Ebs {
  public:
    // Initialize system and variables, but don't kick off any interrupts or loops yet
    Ebs();

    // Start all interrupts and timers that run independent of the main loop & load all previous values
    void start();

    // Run main loop
    void run();

    // Hardware
    Gyroscope gyro;
    FeedbackServo servo;
    Led battery_led;
    Led mode_led;
    Led calibration_led;

  private:
    // State
    bool is_initialized; // If false, no data in eerom. Below variables invalid.
    int curr_gear;
    int num_gears;
    // To shift to a certain gear
    int gear_to_shift_angle[];
    // Best angle while at a certain gear
    int gear_to_ideal_angle[];
};


#endif //EBS_H
