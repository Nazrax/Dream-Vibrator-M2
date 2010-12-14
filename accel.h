#ifndef _ACCEL_H_
#define _ACCEL_H_

#include <avr/io.h>

#define accel_select() PORTD &= ~(_BV(PORTD4))
#define accel_deselect() PORTD |= _BV(PORTD4)

void accel_init(void);
int8_t accel_read(uint8_t);
void accel_take_reading(void);

#endif
