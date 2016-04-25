#include "internal_serial.h"
#include <EEPROM.h>
#include "Ebs.h"
#include "hardware_const.h"
#include "system_const.h"
#include "interrupts.h"

Ebs::Ebs() :
  gyro(),
  servo(HardwareConst::SERVO_ATTACH_PIN, HardwareConst::SERVO_FEEDBACK_PIN),
  battery_led(HardwareConst::BATTERY_LED_PIN),
  mode_led(HardwareConst::MODE_LED_PIN),
  calibration_led(HardwareConst::CALIBRATION_LED_PIN),

  is_initialized(false),

  // Default min/max servo angles for manual mode if loading state fails
  // Guarantees that user can shift even if microcontroller crashes and loses EEPROM while riding
  min_angle(CalibrateConst::ABSOLUTE_MIN_SERVO_ANGLE),
  max_angle(CalibrateConst::ABSOLUTE_MAX_SERVO_ANGLE),

  mode(NORMAL) // Always start in normal mode, other modes make no sense
{
}

void Ebs::start() {
  setup_interrupts();
  load_state();
  print_state();
}

void Ebs::run() {
  // Start bike in default state
  if (is_initialized) {
    // Bike is already in some gear, set servo to that angle
    servo.set_angle(gear_to_shift_angle[curr_gear]);
  } else {
    // Middle angle is likely to be in range. If user must, they can use manual mode
    servo.set_angle(CalibrateConst::MIDDLE_ANGLE);
  }

  // The main loop has to check for requested changes (via interrupts)
  // and execute the requested functions.
  // In addition when no action is in progress, it must keep the LEDs blinking correctly
  update_often();

  // Useful for testing
  if (SystemConst::CALIBRATE_ON_STARTUP) {
    requested_calibration = 1;
  }

  while (1) {

    // Check interrupt request variables
    if (requested_calibration || requested_mode_change || requested_upshift || requested_downshift) {
      handle_request();

      // If we handle a request, we want to clear the flag so that we don't handle it on next loop
      // If there's some other unhandled request after we handled a request, the button got pressed during
      // the action. The user likely didn't mean to queue another action, so we clear it too.
      requested_calibration = 0;
      requested_mode_change = 0;
      requested_upshift = 0;
      requested_downshift = 0;
    }

    // LED handling while not in action
    update_often();

    // Avoid max CPU usage while idle
    delay(1);
  }
}

void Ebs::print_state() {
  PRINTLN("EBS STATE");
  PRINTLN("------------------");
  PRINTLN("Is initialized: " + String(is_initialized));

  // If not initialized, these values are all garbage
  if (is_initialized) {
    PRINTLN("Current gear (1-indexed): " + String(curr_gear + 1));
    PRINTLN("Min angle: " + String(min_angle));
    PRINTLN("Max angle: " + String(max_angle));
    PRINTLN("Num gears: " + String(num_gears));

    // Print gear table for debugging
    PRINTLN("------------------");
    for (int i = 0; i < num_gears; i++) {
      // One-index for the user
      PRINTLN(String(i + 1) + ":\t" + gear_to_shift_angle[i]);
    }
  }
  PRINTLN("------------------");
}

void Ebs::load_state() {
  PRINTLN("Loading state from EEPROM...");
  uint8_t at_check = EEPROM.read(StoreConst::ADDR_CHECK);
  if (at_check != StoreConst::CHECK_VALUE) {
    // EEPROM is empty or corrupted
    is_initialized = false;
    PRINTLN("No valid state found");
    return;
  }

  is_initialized = true;

  // Load remaining values
  curr_gear = EEPROM.read(StoreConst::ADDR_CURR_GEAR);
  num_gears = EEPROM.read(StoreConst::ADDR_NUM_GEARS);
  min_angle = EEPROM.read(StoreConst::ADDR_MIN_ANGLE);
  max_angle = EEPROM.read(StoreConst::ADDR_MAX_ANGLE);

  if (num_gears > SystemConst::GEAR_ANGLE_ARR_SIZE) {
    PRINTLN("WARNING: Read failure; num_gears too high. num_gears set to valid value");
    num_gears = SystemConst::GEAR_ANGLE_ARR_SIZE;
  }

  // Load variable-length array
  for (int i = 0; i < num_gears; i++) {
    gear_to_shift_angle[i] = EEPROM.read(StoreConst::ADDR_ARR_START + i);
  }

  PRINTLN("Read entire state");
}

void Ebs::write_state() {
  PRINTLN("Writing state into EEPROM");

  // Special case: uninitialized, so write just one value to signal this
  if (!is_initialized) {
    EEPROM.update(StoreConst::ADDR_CHECK, StoreConst::BLANK_VALUE);
    return;
  }

  // Big enough to hold all fixed variables and our array
  const size_t WRITE_ARR_SIZE = StoreConst::ADDR_ARR_START + SystemConst::GEAR_ANGLE_ARR_SIZE;
  uint8_t write_arr[WRITE_ARR_SIZE];
  memset(write_arr, StoreConst::BLANK_VALUE, WRITE_ARR_SIZE);

  // Make sure EEPROM is big enough for our data
  if (EEPROM.length() < WRITE_ARR_SIZE) {
    PRINTLN("Error writing state: EEPROM too small");
    return;
  }

  // Fill in values
  write_arr[StoreConst::ADDR_CHECK] = StoreConst::CHECK_VALUE;
  // All these should fit in bytes
  write_arr[StoreConst::ADDR_CURR_GEAR] = (uint8_t) curr_gear;
  write_arr[StoreConst::ADDR_NUM_GEARS] = (uint8_t) num_gears;
  write_arr[StoreConst::ADDR_MIN_ANGLE] = (uint8_t) min_angle;
  write_arr[StoreConst::ADDR_MAX_ANGLE] = (uint8_t) max_angle;
  // Fill in variable-size data
  for (int i = 0; i < num_gears && i < SystemConst::GEAR_ANGLE_ARR_SIZE; i++) {
    write_arr[StoreConst::ADDR_ARR_START + i] = (uint8_t) gear_to_shift_angle[i];
  }

  // Write without cost if data hasn't changed
  EEPROM.put(StoreConst::ADDR_CHECK, write_arr);
}

void Ebs::update_often() {
  servo.refresh();
  update_leds();
}

void Ebs::update_leds() {
  unsigned long curr_time = millis();

  // Calibration mode is easy. It stays solid if calibration is enabled.
  // Solid is easier for the user to understand so that they know when calibration has finished
  calibration_led.set_state(mode == CALIBRATION);

  // The mode LED glow is also easy.
  // It stays on for 3000 ms immediately after a change from normal to manual or vice versa
  // Also blinks during manual mode, so we need to save this for blinking section
  bool mode_led_glow = curr_time <= time_stop_mode_led_glow;

  // Hard part: LED is on for a while then off for a while to establish a blink pattern
  const unsigned long TOTAL_LED_CYCLE_TIME = SystemConst::LED_TIME_ON + SystemConst::LED_TIME_OFF;
  unsigned long curr_time_in_cycle = curr_time % TOTAL_LED_CYCLE_TIME;
  // "on" time is defined at the beginning of the cycle
  bool is_blink_cycle_on = curr_time_in_cycle < SystemConst::LED_TIME_ON;

  // Mode LED is on if it should be glowing or if it's on its blink cycle during manual mode
  mode_led.set_state(mode_led_glow || (mode == MANUAL && is_blink_cycle_on));

  // Battery LED is on if on blink cycle and battery is running low
  battery_led.set_state(is_blink_cycle_on && is_battery_low());
}

bool Ebs::is_battery_low() {
  // TODO: Skeleton, how do we detect battery voltage?
  // Use LOW_BATTERY_THRESHOLD_VOLTS
  return true;
}

void Ebs::toggle_mode() {
  if (mode == CALIBRATION) {
    // This should not have been called; calibration is different
    PRINTLN("Failed mode change: no change allowed during calibration");
    return;
  }

  if (mode == NORMAL) {
    mode = MANUAL;
    PRINTLN("Changed to manual mode");
  } else if (mode == MANUAL) {
    mode = NORMAL;
    PRINTLN("Changed to normal mode");
  }

  // LED should be on for the next 3000ms
  time_stop_mode_led_glow = millis() + SystemConst::MODE_LED_TIME_GLOW;
}

void Ebs::upshift() {
  if(!is_initialized) {
    PRINTLN("Cannot shift before calibrating. Press button on board to run calibration process.");
  }

  if (curr_gear == num_gears - 1) {
    PRINTLN("Already at max gear, cannot upshift");
  }

  change_gear(curr_gear + 1, curr_gear);
}

void Ebs::downshift() {
  if(!is_initialized) {
    PRINTLN("Cannot shift before calibrating. Press button on board to run calibration process.");
  }

  if (curr_gear == 0) {
    PRINTLN("Already at min gear, cannot downshift");
  }

  change_gear(curr_gear - 1, curr_gear);
}

void Ebs::change_gear(int to_gear, int from_gear) {
  int final_angle = gear_to_shift_angle[to_gear];

  int overshoot_by;
  if (from_gear < to_gear) {
    overshoot_by = SystemConst::ANGLE_OVERSHOOT_FORWARD_BY;
  } else {
    overshoot_by = -1 * SystemConst::ANGLE_OVERSHOOT_BACKWARD_BY;
  }

  servo.set_angle(final_angle + overshoot_by);

  // Hold this servo angle for x time to wait for the gear to shift
  productive_delay(SystemConst::HOLD_OVERSHOOT_SEC * 1000);

  servo.set_angle(final_angle);
}

void Ebs::manual_increase() {
  int old_angle = servo.get_last_written_angle();
  int new_angle = old_angle + SystemConst::MANUAL_MODE_DEGREE_CHANGE;

  servo.set_angle(constrain(new_angle, min_angle, max_angle));
}

void Ebs::manual_decrease() {
  int old_angle = servo.get_last_written_angle();
  int new_angle = old_angle - SystemConst::MANUAL_MODE_DEGREE_CHANGE;

  servo.set_angle(constrain(new_angle, min_angle, max_angle));
}

void Ebs::productive_delay(unsigned long wait_ms) {
  unsigned long stop_delay_at = millis() + wait_ms;
  // Just in case this time has already passed, we still want to update everything
  update_often();
  while (millis() < stop_delay_at) {
    update_often();
    delay(1);
  }
}

void Ebs::handle_request() {
  if (requested_calibration) {
    mode = CALIBRATION;
    calibrate();
    // Normal is the natural mode to return to
    mode = NORMAL;
  }
  else if (requested_mode_change) {
    toggle_mode();
  }

  else if (requested_upshift) {
    // Depends on mode
    if (mode == NORMAL) {
      upshift();
    } else if (mode == MANUAL) {
      manual_increase();
    }
  }
  else if (requested_downshift) {
    // Depends on mode
    if (mode == NORMAL) {
      downshift();
    } else if (mode == MANUAL) {
      manual_decrease();
    }
  }
}
