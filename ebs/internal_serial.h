//
// Includes Serial.h or a null-object that does nothing depending on settings
//

#ifndef EBS_INTERNAL_SERIAL_H
#define EBS_INTERNAL_SERIAL_H

#include <WString.h>
#include "system_const.h"

#if USE_SERIAL == 1
#include "HardwareSerial.h"
#else

class SerialClass {
  public:
    void begin(int) {};
    void println(const char*) {};
    void println(String) {};
};

extern SerialClass Serial;

#endif

#endif //EBS_INTERNAL_SERIAL_H
