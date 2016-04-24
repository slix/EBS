#include "internal_serial.h"

#if USE_SERIAL == 0
// Instantiate singleton
SerialClass Serial;
#endif
