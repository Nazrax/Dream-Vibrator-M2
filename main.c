#include "globals.h"
#include "serial.h"
#include "clock.h"
#include "power.h"
#include "accel.h"
#include "spi.h"
#include "flash.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <stdio.h>

// Function prototypes
void init_io(void);
void init_usart(void);

void DoCalibrate(void);

void init_io() {
  DDRB |= _BV(DDB2); // Set SS as output
}

int main(int argc, char** argv) {
  //int i;
  CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable
  CLKPR = _BV(CLKPS0); // Divide by 2, 4 MHz

  DDRB |= _BV(DDB1);
  PORTB = _BV(PORTB1);

  _delay_ms(500);
  
  DoCalibrate();

  PORTB &= ~(_BV(PORTB1));

  init_io();
  usart_init();
  clock_init();
  spi_init();
  flash_init();
  
  sei();
  
  strcpy_P(serial_out, PSTR("\r\nInitializing accelerometer\r\n"));
  usart_send();
  while (flag_serial_sending);
  accel_init();

  strcpy_P(serial_out, PSTR("\r\nScanning Flash\r\n"));
  usart_send();
  while (flag_serial_sending);
  flash_scan();

  sprintf(serial_out, "Flash address: %d\r\n", flash_addr);
  usart_send();
  while (flag_serial_sending);

  strcpy_P(serial_out, PSTR("Ready, scanning\r\n"));
  usart_send();

  flag_accel_enabled = true;

  while(true) {

    clock_update();

    flash_doheader();
    accel_take_reading();
    flash_condwrite();
    usart_dostuff();

    power_sleep();

  } // while(true)

  return 0;
}

