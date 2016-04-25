#include "internal_serial.h"
#include "Ebs.h"
#include "system_const.h"

void Ebs::calibrate() {
  PRINTLN("Starting calibration...");
  PRINTLN("Rotate the pedals until the board LED turns off!");

  min_angle = find_min_angle();

  curr_gear = 0;
  // min_angle is good for first gear
  gear_to_shift_angle[curr_gear++] = min_angle;

  // Loop while rotating gyro
  unsigned long start_time_ms = millis();
  // Equivalent to never shifted
  float last_shift = -999.0f;

  int max_reported_angle = 0;
  int max_input_angle = min_angle;
  float last_hit_sec = 0.0f;

  int last_announced_angle = -99;

  while (1) {
    float elapsed_sec = float(millis() - start_time_ms) / 1000;

    // Rotate servo continuously
    int angle = int(min_angle + (elapsed_sec * CalibrateConst::ROTATE_DEG_PER_SEC));
    servo.set_angle(angle);

    // For sanity during debugging, print angle we've reached at every multiple of x
    if (angle % CalibrateConst::ANNOUNCE_INTERVAL == 0 && angle != last_announced_angle) {
      PRINTLN("At angle " + String(angle));
      last_announced_angle = angle;
    }

    // Detect shifts
    float velocity = gyro.read_z();
    if ((last_shift + CalibrateConst::SHIFT_COOLDOWN_SEC) < elapsed_sec
        && abs(velocity) >= CalibrateConst::SHIFT_DETECT_THRESHOLD) {
      PRINTLN("Shift detected for gear " + String(curr_gear+1) + " at servo angle "
                     + String(angle) + " from gyro velocity " + String(velocity));
      last_shift = elapsed_sec;
      // Also reset the rangefind timeout threshold. If we just switched gears, we're nowhere near the max angle
      last_hit_sec = elapsed_sec;
      // And of course the minimum angle must be at least this detection
      max_input_angle = angle;

      // Check for overflow
      if (curr_gear >= SystemConst::GEAR_ANGLE_ARR_SIZE) {
        PRINTLN("Gear ignored; exceeded array storage");
        curr_gear++;
      } else {
        gear_to_shift_angle[curr_gear++] = angle;
      }
    }

    // Track max seen
    int reported_angle = servo.get_feedback_angle();
    if (reported_angle > max_reported_angle) {
      max_reported_angle = reported_angle;
      max_input_angle = angle;
      last_hit_sec = elapsed_sec;
    }

    // No updated max for a while means we reached max
    // Make sure at least one gear shift has occurred before giving up
    if (num_gears > 1 &&
        elapsed_sec - last_hit_sec >= CalibrateConst::RANGEFIND_TIMEOUT_THRESHOLD_SECONDS) {
      PRINTLN("Detected max physical servo angle at " + String(max_input_angle));
      max_angle = max_input_angle;
      break;
    }
    if (angle >= CalibrateConst::ABSOLUTE_MAX_SERVO_ANGLE) {
      PRINTLN("Reached max physical servo angle at " + String(angle)
                     + " without triggering timeout threshold");
      max_angle = CalibrateConst::ABSOLUTE_MAX_SERVO_ANGLE;
      break;
    }

    update_often();
    productive_delay(CalibrateConst::LOOP_DELAY_MS);
  }

  // ----------
  // CALIBRATION COMPLETE
  // ----------
  PRINTLN("Calibration complete! " + String(curr_gear) + " gears detected.");

  // We only want to remember the number of gears we were able to store
  num_gears = min(curr_gear, SystemConst::GEAR_ANGLE_ARR_SIZE);

  // After calibration: put system in a consistent state (at last gear)
  curr_gear = num_gears - 1;
  servo.set_angle(gear_to_shift_angle[curr_gear]);
  productive_delay(CalibrateConst::END_ADJUST_WAIT * 1000);
  is_initialized = true;

  write_state();
  print_state();
}


int Ebs::find_min_angle() {
  // Feedback sensor on our servo behaves very strangely
  // If servo rotated to 0, angle reported is significantly larger than actual position
  // We must slowly rotate servo to 0 and find out when feedback servo stops reporting changes
  // Because the feedback value is meaningless except as a way to detect changes
  // Other feedback quirks: suddenly increases by ~10 after minimum angle has been overshot
  // And even if the servo is still, it tends to randomly report higher values once per second.

  // Start at angle we know is reachable
  int start_angle = CalibrateConst::MIDDLE_ANGLE;
  servo.set_angle(start_angle);
  productive_delay(CalibrateConst::MIDDLE_ROTATE_WAIT * 1000);

  unsigned long start_time_ms = millis();

  int min_reported_angle = 999;
  int min_input_angle = start_angle;
  float last_hit_sec = 0.0f;

  int last_announced_angle = -99;

  while (1) {
    float elapsed_sec = float(millis() - start_time_ms) / 1000;

    // Rotate continuously but in opposite direction
    int angle = int(start_angle + (elapsed_sec * -CalibrateConst::ROTATE_DEG_PER_SEC));
    servo.set_angle(angle);

    if (angle % CalibrateConst::ANNOUNCE_INTERVAL == 0 && angle != last_announced_angle) {
      PRINTLN("At angle " + String(angle));
      last_announced_angle = angle;
    }

    // Track min seen
    int reported_angle = servo.get_feedback_angle();
    if (reported_angle < min_reported_angle) {
      min_reported_angle = reported_angle;
      min_input_angle = angle;
      last_hit_sec = elapsed_sec;
    }

    // No updated min for a while means we reached min
    if (elapsed_sec - last_hit_sec >= CalibrateConst::RANGEFIND_TIMEOUT_THRESHOLD_SECONDS) {
      PRINTLN("Detected min physical servo angle at " + String(min_input_angle));
      return min_input_angle;
    }
    if (angle <= CalibrateConst::ABSOLUTE_MIN_SERVO_ANGLE) {
      PRINTLN("Reached min physical servo angle at " + String(angle)
                     + " without triggering timeout threshold");
      return CalibrateConst::ABSOLUTE_MIN_SERVO_ANGLE;
    }

    update_often();
  }
}

