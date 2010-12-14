#include "globals.h"
#include "accel.h"
#include "spi.h"
#include "serial.h"

#include <util/delay.h>

#include <stdio.h>

#define XOUT8_REGISTER 0x06
#define YOUT8_REGISTER 0x07
#define ZOUT8_REGISTER 0x08
#define MODE_REGISTER 0x16
#define I2C_ADDRESS_REGISTER 0x0d

#define write_addr(x) ( (1<<7 ) | ((x) << 1))
#define read_addr(x) ((x) << 1)

int8_t x_arry[5], y_arry[5], z_arry[5];

void accel_take_reading() {
  if (flag_want_reading) {
    int8_t x,y,z;
   
    x = accel_read(0);
    y = accel_read(1);
    z = accel_read(2);

    flash_buf[flash_buf_ctr++] = x;
    flash_buf[flash_buf_ctr++] = y;
    flash_buf[flash_buf_ctr++] = z;
    
    sprintf(serial_out, "X: %d Y: %d Z: %d\r\n", x, y, z);
    usart_send();

    flag_want_reading = false;
  }
}

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

int8_t accel_read(uint8_t i) {
  int8_t reading;

  accel_select();
  spi_send(read_addr(XOUT8_REGISTER + i));
  reading = (int8_t)spi_send(DONTCARE);
  accel_deselect();

  return reading;
}

