//
// ATTiny should use a SoftwareServo
//

#ifndef EBS_INTERNAL_SERVO_H
#define EBS_INTERNAL_SERVO_H

#include "system_const.h"

#if !USE_SOFTWARESERVO
#include <Servo.h>
#else
#include "lib/SoftwareServo.h"
#define Servo SoftwareServo
#endif

#endif //EBS_INTERNAL_SERVO_H
