#include "globals.h"
#include "spi.h"
#include "accel.h"
#include "serial.h"

uint8_t spi_send(uint8_t data) {
  uint8_t retval;

  /*
  sprintf(serial_out, "SPI Sending: %d\r\n", data);
  usart_send();
  while(flag_serial_sending);
  */

  SPDR = data;
  while (!(SPSR & _BV(SPIF)));
  retval = SPDR;

  /*
  sprintf(serial_out, "SPI Got: %d\r\n", retval);
  usart_send();
  while(flag_serial_sending);
  */

  return retval;
}

void spi_init(void) {
  DDRB |= _BV(PORTB2) | _BV(PORTB3) | _BV(PORTB5); // /SS, MOSI, and SCK are output
  SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0); // Enable SPI, F_CPU / 16
  accel_deselect();
}
