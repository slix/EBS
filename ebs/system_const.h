#ifndef SYSTEM_CONST_H
#define SYSTEM_CONST_H

namespace CalibrateConst {
  // Rate of rotation of derailleur servo
  const float ROTATE_DEG_PER_SEC = 3.0f;

  // Threshold in deg/sec. Beyond this threshold, a shift is detected
  const float SHIFT_DETECT_THRESHOLD = 195.6525;

  // During a shift, the gyro threshold will be hit multiple times a second (despite no new shift)
  // Ignore shift data for this long after a shift
  const float SHIFT_COOLDOWN_SEC = 1.0f;

  // Servo cannot go beyond this range
  const int ABSOLUTE_MIN_SERVO_ANGLE = 0;
  const int ABSOLUTE_MAX_SERVO_ANGLE = 180;

  // Once we haven't seen new min/max angles for this many seconds, give up finding more gears
  // Servo likely hit physical min/max angle
  const float RANGEFIND_TIMEOUT_THRESHOLD_SECONDS = 2.5f;

  // Max number of gears our program will store
  const int GEAR_ANGLE_ARR_MAX = 15;

  // An angle we know is physically reachable
  const int MIDDLE_ANGLE = 90;
  // Wait this many seconds for the servo to reach the middle angle
  const float MIDDLE_ROTATE_WAIT = 2.0f;

  // At what multiples of angles to announce the current angle
  const int ANNOUNCE_INTERVAL = 5;
}

#endif //SYSTEM_CONST_H
