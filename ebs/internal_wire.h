//
// Use Wire.h for Arduino Uno and TinyWire for ATTiny
//

#ifndef EBS_INTERNAL_WIRE_H
#define EBS_INTERNAL_WIRE_H

#include "system_const.h"

#if USE_TINYWIRES == 0
#include "Wire.h"
#else
#include "lib/TinyWireM.h"
#define Wire TinyWireM
#endif

#endif //EBS_INTERNAL_WIRE_H
