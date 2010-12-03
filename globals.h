#ifndef _MAIN_H_
#define _MAIN_H_

#include <avr/io.h>

#include "types.h"

#define REGISTER_BIT(...) REGISTER_BIT_(__VA_ARGS__)
#define REGISTER_BIT_(rg,bt) ((volatile _io_reg*)&rg)->bit##bt 

#define F_CPU 3686400
#define BAUD_VALUE 1 // 230.4k @  3.6864MHz

// Serial stuff
extern char serial_out[132];
extern volatile char serial_in[64];
extern volatile uint8_t serial_in_ctr;
extern volatile uint8_t serial_out_ctr;


// GPIOR0
#define SERIAL_SENDING GPIOR0, 0
#define WANT_READING GPIOR0, 1

// Flags
#define flag_serial_sending REGISTER_BIT(SERIAL_SENDING)
#define flag_want_reading REGISTER_BIT(WANT_READING)

// Clock stuff
extern clock_t clock;
extern volatile uint32_t clock_ticks;

#endif
