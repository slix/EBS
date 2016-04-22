#include <HardwareSerial.h>
#include <EEPROM.h>
#include "Ebs.h"
#include "hardware_const.h"
#include "system_const.h"

Ebs::Ebs() :
  gyro(),
  servo(HardwareConst::SERVO_ATTACH_PIN, HardwareConst::SERVO_FEEDBACK_PIN),
  battery_led(HardwareConst::BATTERY_LED_PIN),
  mode_led(HardwareConst::MODE_LED_PIN),
  calibration_led(HardwareConst::CALIBRATION_LED_PIN),

  is_initialized(false)
{
}

void Ebs::start() {
  print_state();
  load_state();
  print_state();
}

void Ebs::run() {
  calibrate();
  // And now nothing
  while (1) {
    delay(1000);
  }
}

void Ebs::print_state() {
  Serial.println("EBS STATE");
  Serial.println("------------------");
  Serial.println("Is initialized: " + String(is_initialized));

  // If not initialized, these values are all garbage
  if (is_initialized) {
    Serial.println("Current gear: " + String(curr_gear));
    Serial.println("Min angle: " + String(min_angle));
    Serial.println("Max angle: " + String(max_angle));
    Serial.println("Num gears: " + String(num_gears));

    // Print gear table for debugging
    Serial.println("------------------");
    for (int i = 0; i < num_gears; i++) {
      // One-index for the user
      Serial.println(String(i + 1) + ":\t" + gear_to_shift_angle[i]);
    }
  }
  Serial.println("------------------");
}

void Ebs::load_state() {
  Serial.println("Loading state from EEPROM...");
  uint8_t at_check = EEPROM.read(StoreConst::ADDR_CHECK);
  if (at_check != StoreConst::CHECK_VALUE) {
    // EEPROM is empty or corrupted
    is_initialized = false;
    Serial.println("No valid state found");
    return;
  }

  is_initialized = true;

  // Load remaining values
  curr_gear = EEPROM.read(StoreConst::ADDR_CURR_GEAR);
  num_gears = EEPROM.read(StoreConst::ADDR_NUM_GEARS);
  min_angle = EEPROM.read(StoreConst::ADDR_MIN_ANGLE);
  max_angle = EEPROM.read(StoreConst::ADDR_MAX_ANGLE);

  if (num_gears > SystemConst::GEAR_ANGLE_ARR_SIZE) {
    Serial.println("WARNING: Read failure; num_gears too high. num_gears set to valid value");
    num_gears = SystemConst::GEAR_ANGLE_ARR_SIZE;
  }

  // Load variable-length array
  for (int i = 0; i < num_gears; i++) {
    gear_to_shift_angle[i] = EEPROM.read(StoreConst::ADDR_ARR_START + i);
  }

  Serial.println("Read entire state");
}

void Ebs::write_state() {
  Serial.println("Writing state into EEPROM");

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
    Serial.println("Error writing state: EEPROM too small");
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
