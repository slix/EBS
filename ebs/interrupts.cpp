// Written by Matt Potok
// Modifications to fit ebs by Peter Kowalczyk

#include <avr/io.h>
#include <USBAPI.h>
#include "interrupts.h"
#include "system_const.h"
#include "internal_serial.h"

// Communication with other files
volatile int requested_calibration = 0;
volatile int requested_mode_change = 0;
volatile int requested_upshift = 0;
volatile int requested_downshift = 0;

#if !ENABLE_INTERRUPTS
void setup_interrupts() {
}
#else

// Shift vars
volatile uint8_t cur_PIND = 0;
volatile int dwn_dep = 0; // Down button pressed
volatile int dwn_unp = 0; // Down button unpressed
volatile int up_dep = 0;  // Up button pressed
volatile int up_unp = 0;  // Up button unpressed
volatile int md_dep = 0;  // Mode button pressed
volatile int md_unp = 0;  // Mode button unpressed

void setup_interrupts() {
  // AVR status register
  SREG |= bit(7); // Enable the Global Interrupt Enable bit

  // Setup interrupt for PCINT[23:16]
  PCIFR  |= bit (PCIF2);  // Clear the PCIFR flags for PCINT[23:16]
  PCICR  |= bit (PCIE2);  // Enable pin change interrupts for D0 to D7

  // Set individual bits on PCINT[7:0]
  PCMSK2 |= bit(PCINT23); // Enable interrupts on PCINT23
  PCMSK2 |= bit(PCINT22); // Enable interrupts on PCINT22
  PCMSK2 |= bit(PCINT21); // Enable interrupts on PCINT21

  PRINTLN("Setup interrupts");
}

ISR(PCINT2_vect) {
  PRINTLN("HERE");
  
  // PIND is used a register used by the Uno
  cur_PIND = (PIND & bit(7) | PIND & bit(6) | PIND & bit(5));
  switch (cur_PIND) {
    // Down/up/md depressed
    case (B11100000):
      dwn_dep = 1;
      up_dep = 1;
      md_dep = 1;
      break;

    // Down/up depressed, md unpressed
    case (B11000000):
      dwn_dep = 1;
      up_dep = 1;
      md_unp = (md_dep) ? 1 : 0;
      break;

    // Down/md depressed, up unpressed
    case (B10100000):
      dwn_dep = 1;
      up_unp = (up_dep) ? 1 : 0;
      md_dep = 1;
      break;

    // Down depressed, up/md unpressed
    case (B10000000):
      dwn_dep = 1;
      up_unp = (up_dep) ? 1 : 0;
      md_unp = (md_dep) ? 1 : 0;
      break;

    // Up/md depressed, down unpressed
    case (B01100000):
      dwn_unp = (dwn_dep) ? 1 : 0;
      up_dep = 1;
      md_dep = 1;
      break;

    // Up depressed, down/md unpressed
    case (B01000000):
      dwn_unp = (dwn_dep) ? 1 : 0;
      up_dep = 1;
      md_unp = (md_dep) ? 1 : 0;
      break;

    // Md depressed, down/up unpressed
    case (B00100000):
      dwn_unp = (dwn_dep) ? 1 : 0;
      up_unp = (up_dep) ? 1 : 0;
      md_dep = 1;
      break;

    // Down/up/md unpressed
    case (B00000000):
      dwn_unp = (dwn_dep) ? 1 : 0;
      up_unp = (up_dep) ? 1 : 0;
      md_unp = (md_dep) ? 1 : 0;
      break;
  }

  // Detecting dwn_btn press
  if (dwn_dep && dwn_unp) {
    requested_downshift = 1;
    dwn_dep = 0;
    dwn_unp = 0;
    PRINTLN("Down button pressed");
  }

  // Detecting up_btn press
  if (up_dep && up_unp) {
    requested_upshift = 1;
    up_dep = 0;
    up_unp = 0;
    PRINTLN("Up button pressed");
  }

  // Detecting both dwn_btn and up_btn depress
  if (up_dep && dwn_dep) {
    requested_mode_change = 1;
    up_dep = 0;
    dwn_dep = 0;
    PRINTLN("Shift mode changed");
  }

  // Detecting mode btn press
  if (md_dep && md_unp) {
    requested_calibration = 1;
    md_dep = 0;
    md_unp = 0;
    PRINTLN("Mode button pressed");
  }
}

#endif
