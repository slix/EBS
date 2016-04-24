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
  Serial.begin(9600);
}

void loop()
{
  Ebs ebs;
  ebs.start();
  ebs.run();
}
