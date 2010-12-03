#include "globals.h"
#include "accel.h"
#include "spi.h"

#define XOUT8_REGISTER 0x06
#define MODE_REGISTER 0x16
#define I2C_ADDRESS_REGISTER 0x0d

#define write_addr(x) ( (1<<7 ) & (x << 1))
#define read_addr(x) (x << 1)

void accel_init() {
  accel_select();
  spi_send(write_addr(MODE_REGISTER)); // Mode register
  spi_send(0b01000101); // Measurement mode, no interrupt, 2g measurement range
  accel_deselect();

  accel_select();
  spi_send(write_addr(I2C_ADDRESS_REGISTER));
  spi_send(0b10000000); // Disable I2C
  accel_deselect();
}

uint32_t accel_read() {
  uint32_t x,y,z;

  accel_select();
  spi_send(read_addr(XOUT8_REGISTER));
  x = spi_send(DONTCARE);
  y = spi_send(DONTCARE);
  z = spi_send(DONTCARE);
  accel_deselect();

  return (x << 16) + (y << 8) + z;
}

