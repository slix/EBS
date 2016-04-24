//
// Exposes button-interrupt based variables to other files
//

#ifndef EBS_INTERRUPTS_H
#define EBS_INTERRUPTS_H

// If any of these are set to 1, the user has requested the given action
// If something is set to 1 during another long-running action, ignore the action (likely a mistake)
extern volatile int requested_calibration;
extern volatile int requested_mode_change;
extern volatile int requested_upshift;
extern volatile int requested_downshift;

// Call during setup to enable interrupts
void setup_interrupts();

#endif //EBS_INTERRUPTS_H
