#include "globals.h"
#include "serial.h"
#include "clock.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// Function prototypes
void init_io(void);
void init_usart(void);

void DoCalibrate(void);

void init_io() {
  DDRB = _BV(DDB2); // Set SS as output
}


int main(int argc, char** argv) {
  CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable
  CLKPR = _BV(CLKPS0); // Divide by 2, 4 MHz
  DoCalibrate();

  init_io();
  clock_init();

  PCICR = _BV(PCIE1) | _BV(PCIE2); // Enable pin change interrupts
  PCMSK2 |= _BV(PCINT16); // Enable pin change interrupt on USART RX

  sei();

  strcpy_P(serial_out, PSTR("\r\nScanning\r\n"));
  usart_send();
  while (flag_serial_sending);
  // Put accelerometer in SPI mode
  strcpy_P(serial_out, PSTR("Ready\r\n"));

  while(true) {
    if (flag_want_reading) {
      // Get readings from accelerometer
      // Print them

      flag_want_reading = false;
    }

    clock_update();
    // Sleep
  }

  return 0;
}




