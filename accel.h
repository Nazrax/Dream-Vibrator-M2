#ifndef _ACCEL_H_
#define _ACCEL_H_

#define accel_select() PORTB &= ~(_BV(PORTB2))
#define accel_deselect() PORTB |= _BV(PORTB2)

uint32_t accel_read(void);
void accel_init(void);

#endif
