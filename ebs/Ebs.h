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

    // Run calibration process to find all gears, TAKES A LONG TIME
    void calibrate();

    // Print full state of shifting system for debugging
    void print_state();

    // Load state into state variables
    void load_state();

    // Write state variables into EEPROM
    // Done such that if nothing has changed, nothing is written
    void write_state();

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
    int min_angle; // Min angle servo can physically rotate to
    int max_angle;
    // To shift to a certain gear
    int* gear_to_shift_angle;

    // LONG algorithm that rotates the servo and finds minimum physical angle
    int find_min_angle();
};


#endif //EBS_H
