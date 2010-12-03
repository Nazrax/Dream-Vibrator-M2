#define F_CPU 1000000

#include "main.h"

// Global variables
uint64_t clock;

// Function prototypes
void init_io(void);
void init_clock(void);

void init_io() {
  DDRB = _BV(DDB2); // Set SS as output
}

void init_clock() {
  ASSR |= _BV(AS2); // Enable async clock
  TCCR2A |= _BV(WGM12); // CTC mode
  OCR2A = 1; // Fire every other clock tick (16 times / second)
  TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20); // Clock / 1024
  TIMSK2 |= _BV(OCIE2A); // Enable CTC interrupt for OCR2A
}

// The clock
ISR(TIMER2_COMPA_vect) {
  clock++;
}

int main(int argc, char** argv) {
  init_io();
  init_clock();

  return 0;
}
