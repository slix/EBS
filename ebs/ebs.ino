/*
 * ebs.ino
 *
 * Author: 
 * Matt Potok
 * Peter Kowalczyk
 */

#include "internal_serial.h"
#include "hardware_const.h"
#include "Ebs.h"



void setup()
{
  // Serial console is important for debugging info
  // Don't need if no serial
  #if USE_SERIAL == 1
  Serial.begin(9600);
  #endif
  PRINTLN("setup");

  // Debugging
 // pinMode(HardwareConst::CALIBRATION_LED_PIN, OUTPUT);
}

void loop()
{
//  // Debugging
Ebs ebs;
  digitalWrite(HardwareConst::CALIBRATION_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(HardwareConst::CALIBRATION_LED_PIN, LOW);
  delay(1000);
  
  
ebs.start();
//
//  // Debugging
digitalWrite(HardwareConst::CALIBRATION_LED_PIN, HIGH);
delay(1000);
digitalWrite(HardwareConst::CALIBRATION_LED_PIN, LOW);
delay(1000);
  
ebs.run();
}
