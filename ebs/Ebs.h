#ifndef EBS_EBS_H
#define EBS_EBS_H

#include "Gyroscope.h"
#include "FeedbackServo.h"
#include "Led.h"
#include "system_const.h"
#include "ebs_mode.h"

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

    // Call AS OFTEN AS POSSIBLE. Blinks LEDs using the current time
    // Should be called on every iteration of every loop in the program
    // Should be called during every delay: see productive_delay()
    void update_leds();

    // Use instead of delay() to update LEDs during the delay
    void productive_delay(unsigned long wait_ms);

    // Toggle mode from normal to manual (or vice versa)
    void toggle_mode();

    // Upshift/downshift into the appropriate gear
    // Does not go beyond gears existing on bike
    void upshift();
    void downshift();

    // Used in manual mode to increase and decrease servo by constant degree on each button press
    void manual_increase();
    void manual_decrease();

    // Hardware
    Gyroscope gyro;
    FeedbackServo servo;
    Led battery_led;
    Led mode_led;
    Led calibration_led;

  private:
    // Current mode of operation
    EbsMode mode;

    // State
    bool is_initialized; // If false, no data in eerom. Below variables invalid.
    int curr_gear;
    int num_gears;
    int min_angle; // Min angle servo can physically rotate to
    int max_angle;
    // To shift to a certain gear
    int gear_to_shift_angle[SystemConst::GEAR_ANGLE_ARR_SIZE];

    // LONG algorithm that rotates the servo and finds minimum physical angle
    int find_min_angle();

    // Increment this by MODE_LED_TIME_GLOW after changing normal->manual or manual->normal
    unsigned long time_stop_mode_led_glow;

    // Returns whether battery is running low
    bool is_battery_low();

    // Change gear to to_gear from from_gear using overshooting
    void change_gear(int to_gear, int from_gear);

    // Call when one of the request variables is true
    // Executes a given action based on user input
    void handle_request();
};


#endif //EBS_H
