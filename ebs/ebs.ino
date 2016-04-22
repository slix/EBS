/*
 * ebs.ino
 *
 * Author: 
 * Matt Potok
 * Peter Kowalczyk
 */

#include <HardwareSerial.h>
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
