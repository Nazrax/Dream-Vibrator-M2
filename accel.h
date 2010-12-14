#ifndef _ACCEL_H_
#define _ACCEL_H_

#define accel_select() PORTD &= ~(_BV(PORTD4))
#define accel_deselect() PORTD |= _BV(PORTD4)

void accel_init(void);
uint32_t accel_read(void);
void accel_take_reading(void);

#endif
