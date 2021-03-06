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
  int i;
  //uint8_t old_mcusr = MCUSR;

  MCUSR = 0;
  wdt_disable();

  CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable
  CLKPR = _BV(CLKPS0); // Divide by 2, 4 MHz

  DDRB |= _BV(DDB1);
  PORTB = _BV(PORTB1);

  _delay_ms(500);
  
  wdt_enable(WDTO_8S);

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

  strcpy_P(serial_out, PSTR("\r\nSleeping for 5 seconds\r\n"));
  usart_send();
  while (flag_serial_sending);
  _delay_ms(5000);

  for(i=0; i<5; i++) {
    strcpy_P(serial_out, PSTR("\r\nScanning Flash\r\n"));
    usart_send();
    while (flag_serial_sending);
    flash_scan();

    sprintf(serial_out, "Flash address: %d\r\n", flash_addr);
    usart_send();
    while (flag_serial_sending);

    if (flash_addr < 4096)
      break;

    strcpy_P(serial_out, PSTR("\r\nFlash claims to be full, trying again ...\r\n"));
    usart_send();
    while (flag_serial_sending);

    _delay_ms(1000);
  }
    

  strcpy_P(serial_out, PSTR("Ready\r\n"));
  usart_send();

  flag_accel_enabled = true;
  flag_accel_verbose = true;

  while(true) {

    clock_update();

    flash_doheader();
    accel_take_reading();
    flash_condwrite();
    usart_dostuff();

    wdt_reset();
    power_sleep();
    wdt_reset();
  } // while(true)

  return 0;
}

