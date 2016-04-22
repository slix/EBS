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

  is_initialized(false),
  gear_to_shift_angle(NULL)
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
    delete[] gear_to_shift_angle;
    gear_to_shift_angle = NULL;
    Serial.println("No valid state found");
    return;
  }

  is_initialized = true;

  // Load remaining values
  curr_gear = EEPROM.read(StoreConst::ADDR_CURR_GEAR);
  num_gears = EEPROM.read(StoreConst::ADDR_NUM_GEARS);
  min_angle = EEPROM.read(StoreConst::ADDR_MIN_ANGLE);
  max_angle = EEPROM.read(StoreConst::ADDR_MAX_ANGLE);

  // Load variable-length array
  delete[] gear_to_shift_angle;
  gear_to_shift_angle = new int[num_gears];
  for (int i = 0; i < num_gears; i++) {
    gear_to_shift_angle[i] = EEPROM.read(StoreConst::ADDR_ARR_START + i);
  }

  Serial.println("Read entire state");
}
