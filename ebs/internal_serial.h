//
// Includes Serial.h or a noop-macro that does nothing depending on settings
//

#ifndef EBS_INTERNAL_SERIAL_H
#define EBS_INTERNAL_SERIAL_H

#include "system_const.h"

// Need a macro to prevent unnecessary string literals from being compiled
// To save space on compiled binary
#if USE_SERIAL == 1
#include "HardwareSerial.h"
#define PRINTLN(x) Serial.println(x)
#else
// No-op macro, string literal should get ignored
#define PRINTLN(x)
#endif

#endif //EBS_INTERNAL_SERIAL_H
