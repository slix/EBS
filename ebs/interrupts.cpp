// Written by Matt Potok
// Modifications to fit ebs by Peter Kowalczyk

#include <avr/io.h>
#include <USBAPI.h>
#include "interrupts.h"

// Communication with other files
volatile int requested_calibration = 0;
volatile int requested_mode_change = 0;
volatile int requested_upshift = 0;
volatile int requested_downshift = 0;

// Shift vars
volatile uint8_t cur_PINA = 0;
// FIXME: can just use a char for all these with bit manipulations
volatile int dwn_dep = 0; // Down button pressed
volatile int dwn_unp = 0; // Down button unpressed
volatile int up_dep = 0;  // Up button pressed
volatile int up_unp = 0;  // Up button unpressed

void setup_interrupts() {
  // AVR status register
  SREG |= bit(7); // Enable the Global Interrupt Enable bit

  // Setup interupt for PCINT[7:0]
  GIFR  |= bit(PCIF0);   // Clear the PCIFR flags for PCINT[7:0]
  GIMSK |= bit(PCIE0);   // Enable pin change interrupts for D0 to D7

  // Set individual bits on PCINT[7:0]
  PCMSK0 |= bit(PCINT0); // Enable interrupts on PCINT0
  PCMSK0 |= bit(PCINT2); // Enable interrupts on PCINT2

  // Setup interrupt for PCINT[11:8]
  GIFR  |= bit(PCIF1);
  GIMSK |= bit(PCIE1);

  // Set individual bits on the PCINT[11:8]
  PCMSK1 |= bit(PCINT9); // Enable interrupts on PCINT9
}

ISR(PCINT0_vect) {
  // PINA is used a register used by the ATtiny84
  cur_PINA = (PINA & bit(PCINT0) | PINA & bit(PCINT2));
  switch (cur_PINA) {
    // Up/down shift depressed
    case (B00000101):
      dwn_dep = 1;
      up_dep = 1;
      break;

      // Down shift depressed
    case (B00000001):
      dwn_dep = 1;
      up_unp = (up_dep) ? 1 : 0;
      break;

      // Down shift depressed
    case (B00000100):
      dwn_unp = (dwn_dep) ? 1 : 0;
      up_dep = 1;
      break;

      // Down shift unpressed
    case (B00000000):
      dwn_unp = (dwn_dep) ? 1 : 0;
      up_unp = (up_dep) ? 1 : 0;
      break;
  }

  // Detecting dwn_btn press
  // sent
  if (dwn_dep && dwn_unp) {
    requested_downshift = 1;
    dwn_dep = 0;
    dwn_unp = 0;
  }

  // Detecting up_btn press
  if (up_dep && up_unp) {
    requested_upshift = 1;
    up_dep = 0;
    up_unp = 0;
  }

  // Detecting both dwn_btn and up_btn depress
  // Make sure unpress doesn't cause shifts
  // TODO: @mattpotok Can you make sure I did this right?
  if (up_dep && dwn_dep) {
    requested_mode_change = 1;
    up_dep = 0;
    dwn_dep = 0;
  }

  // TODO: Calibration button handling
}
