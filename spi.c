#include "globals.h"
#include "spi.h"
#include "accel.h"

uint8_t spi_send(uint8_t data) {
  uint8_t retval;

  SPDR = data;
  while (!(SPSR & _BV(SPIF)));
  retval = SPDR;

  return retval;
}

void init_spi(void) {
  DDRB |= _BV(PORTB2) | _BV(PORTB3) | _BV(PORTB5); // /SS, MOSI, and SCK are output
  SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0); // Enable SPI, F_CPU / 16
  accel_deselect();
}
