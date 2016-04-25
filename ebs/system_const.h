#ifndef SYSTEM_CONST_H
#define SYSTEM_CONST_H

#include <avr/io.h>

#define ATTINY_MODE 0

// Enable Serial.h. Disable for the attiny, where it is not available
#define USE_SERIAL !ATTINY_MODE

// Enable TinyWires instead of Wire.h (should be 1 on attiny)
#define USE_TINYWIRES ATTINY_MODE

// Enable SoftwareServo instead (1 on attiny)
#define USE_SOFTWARESERVO ATTINY_MODE

// Use attiny interrupts instead of Uno interrupts
#define USE_ATTINY_INTERRUPTS ATTINY_MODE

// Use attiny pin definitions instead of Uno definitions
#define USE_ATTINY_PINS ATTINY_MODE

// Enable button interrupts. Disable if you don't have working interrupts for the platform
#define ENABLE_INTERRUPTS 0

namespace CalibrateConst {
  // Rate of rotation of derailleur servo
  const float ROTATE_DEG_PER_SEC = 3.0f;

  // Threshold in deg/sec. Beyond this threshold, a shift is detected
  const float SHIFT_DETECT_THRESHOLD = 195.6525;

  // During a shift, the gyro threshold will be hit multiple times a second (despite no new shift)
  // Ignore shift data for this long after a shift
  const float SHIFT_COOLDOWN_SEC = 1.0f;

  // Servo cannot go beyond this range
  const int ABSOLUTE_MIN_SERVO_ANGLE = 0;
  const int ABSOLUTE_MAX_SERVO_ANGLE = 180;

  // Once we haven't seen new min/max angles for this many seconds, give up finding more gears
  // Servo likely hit physical min/max angle
  const float RANGEFIND_TIMEOUT_THRESHOLD_SECONDS = 2.5f;

  // An angle we know is physically reachable
  const int MIDDLE_ANGLE = 90;
  // Wait this many seconds for the servo to reach the middle angle
  const float MIDDLE_ROTATE_WAIT = 2.0f;

  // Wait this many seconds for the servo to re-adjust itself to the last gear after end of calibration
  const float END_ADJUST_WAIT = 1.0f;

  // At what multiples of angles to announce the current angle
  const int ANNOUNCE_INTERVAL = 5;

  // This prevents the gyro from glitching out during the loop
  const unsigned long LOOP_DELAY_MS = 10;
}

// The addresses where we store each variable in EEPROM
namespace StoreConst {
  // Where to check for correctness and initialization
  const int ADDR_CHECK = 0;
  // Correctness value. If anything other than this, corrupted or uninitialized
  // If you want to start from scratch (new storage format), set this to something else
  const uint8_t CHECK_VALUE = 42;
  // By default, EEPROM values are 255. Setting blank things to 255 may save write cycles
  const uint8_t BLANK_VALUE = 255;

  const int ADDR_CURR_GEAR = 1;
  const int ADDR_NUM_GEARS = 2;
  const int ADDR_MIN_ANGLE = 3;
  const int ADDR_MAX_ANGLE = 4;

  // CRITICAL: MUST BE RIGHT AFTER MAXIMUM ADDRESS SET ABOVE. OVERLAP OR BLANK SPACE PROHIBITED
  // Where the variable size (num_gears) array starts
  const int ADDR_ARR_START = 5;
}

namespace SystemConst {
  // If true, automatically run a calibration on boot, useful for debugging without buttons
  const bool CALIBRATE_ON_STARTUP = true;

  // Max number of gears our program will store
  const int GEAR_ANGLE_ARR_SIZE = 15;

  // Time in ms after a normal->manual or manual->normal change that the mode change LED will stay on
  const unsigned long MODE_LED_TIME_GLOW = 3000;

  // Defines LED blink cycles
  const unsigned long LED_TIME_ON = 100;
  const unsigned long LED_TIME_OFF = 1900;

  const float LOW_BATTERY_THRESHOLD_VOLTS = 5.9f;

  // Overshoot by this many degrees when increasing gear
  const int ANGLE_OVERSHOOT_FORWARD_BY = 6;
  // Overshoot by this many degrees when decreasing gear
  const int ANGLE_OVERSHOOT_BACKWARD_BY = 6;
  // Hold overshoot position for this many seconds before returning
  const float HOLD_OVERSHOOT_SEC = 1.0f;

  // How many degrees to change in manual mode
  const int MANUAL_MODE_DEGREE_CHANGE = 2;
}

#endif //SYSTEM_CONST_H
