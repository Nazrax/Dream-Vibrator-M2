#include "globals.h"
#include "accel.h"
#include "spi.h"
#include "serial.h"

#include <util/delay.h>

#define XOUT8_REGISTER 0x06
#define YOUT8_REGISTER 0x07
#define ZOUT8_REGISTER 0x08
#define MODE_REGISTER 0x16
#define I2C_ADDRESS_REGISTER 0x0d

#define write_addr(x) ( (1<<7 ) | (x << 1))
#define read_addr(x) (x << 1)

int8_t x_arry[5], y_arry[5], z_arry[5];

void accel_init() {
  DDRD |= _BV(DDD4); // !CS
  accel_deselect();

  _delay_ms(100);
  
  accel_select();
  spi_send(write_addr(MODE_REGISTER)); // Mode register
  spi_send(0b01000101); // Measurement mode, no interrupt, 2g measurement range
  accel_deselect();

  accel_select();
  spi_send(write_addr(I2C_ADDRESS_REGISTER));
  spi_send(0b10000000); // Disable I2C
  accel_deselect();
  
}

uint32_t accel_read2() {
  uint32_t x,y,z;

  x = 1;
  y = 2;
  z = 3;

  return (x << 16) + (y << 8) + z;
}

uint32_t accel_read() {
  uint32_t x,y,z;
  uint8_t i;
  uint32_t xa = 0, ya = 0, za = 0;

  accel_select();
  spi_send(read_addr(XOUT8_REGISTER));
  x = spi_send(DONTCARE);
  accel_deselect();


  accel_select();
  spi_send(read_addr(YOUT8_REGISTER));
  y = spi_send(DONTCARE);
  accel_deselect();


  accel_select();
  spi_send(read_addr(ZOUT8_REGISTER));
  z = spi_send(DONTCARE);
  accel_deselect();

  /*
  for(i=1; i<5; i++) {
    x_arry[i] = x_arry[i-1];
    y_arry[i] = y_arry[i-1];
    z_arry[i] = z_arry[i-1];
  }

  x_arry[0] = x;
  y_arry[0] = y;
  z_arry[0] = z;

  for(i=0; i<5; i++) {
    xa += x_arry[i];
    ya += y_arry[i];
    za += z_arry[i];
  }
  xa /= 5;
  ya /= 5;
  za /= 5;
  */
  return (x << 16) | (y << 8) | z;
}

