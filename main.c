#include "globals.h"
#include "serial.h"
#include "clock.h"
#include "power.h"
#include "accel.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <stdio.h>

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

  strcpy_P(serial_out, PSTR("\r\nInitializing accelerometer\r\n"));
  usart_send();
  while (flag_serial_sending);
  accel_init();
  strcpy_P(serial_out, PSTR("Ready\r\n"));

  while(true) {
    if (flag_want_reading) {
      uint32_t reading;
      uint8_t x,y,z;

      reading = accel_read();
      x = reading >> 16;
      y = (reading >> 8) & 0xff;
      z = reading & 0xff;

      sprintf(serial_out, "X: %d Y: %d Z: %d\n", x, y, z);
      usart_send();

      flag_want_reading = false;
    }

    clock_update();

    power_sleep();
  }

  return 0;
}




