#include "globals.h"

#include <avr/io.h>
#include <avr/interrupt.h>


// Global variables
uint64_t clock;
bool_t doReading;

// Function prototypes
void init_io(void);
void init_clock(void);
void init_usart(void);

void DoCalibrate(void);

void init_io() {
  DDRB = _BV(DDB2); // Set SS as output
}

void init_clock() {
  ASSR |= _BV(AS2); // Enable async clock
  TCCR2A |= _BV(WGM21); // CTC mode
  OCR2A = 1; // Fire every other clock tick (16 times / second)
  TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20); // Clock / 1024
  TIMSK2 |= _BV(OCIE2A); // Enable CTC interrupt for OCR2A
}

inline void init_usart(void) {
  UBRR0 = BAUD_VALUE;
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01); // 8-bit, 1 stop bit, no parity
  UCSR0B = _BV(TXEN0) | _BV(TXCIE0) | _BV(RXEN0) | _BV(RXCIE0); // Turn on TX and RX and their interrupts
  UCSR0A = _BV(U2X0); // Double serial clock
}


int main(int argc, char** argv) {
  CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable
  CLKPR = _BV(CLKPS0); // Divide by 2, 4 MHz
  DoCalibrate();

  init_io();
  init_clock();

  sei();

  // Put accelerometer in SPI mode

  while(true) {
    if (doReading) {
      // Get readings from accelerometer
      // Print them

      doReading = false;
    }
  }

  return 0;
}

// The clock
ISR(TIMER2_COMPA_vect) {
  clock++;
}


