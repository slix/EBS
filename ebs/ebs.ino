/*
 * ebs.ino
 *
 * Author: 
 * Matt Potok
 * Peter Kowalczyk
 */

#include "internal_serial.h"
#include "Ebs.h"

void setup()
{
  // Serial console is important for debugging info
  // Don't need if no serial
  #if USE_SERIAL == 1
  Serial.begin(9600);
  #endif
}

void loop()
{
  Ebs ebs;
  ebs.start();
  ebs.run();
}
