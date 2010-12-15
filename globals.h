#ifndef _MAIN_H_
#define _MAIN_H_

#include <avr/io.h>

#include "types.h"

#define REGISTER_BIT(...) REGISTER_BIT_(__VA_ARGS__)
#define REGISTER_BIT_(rg,bt) ((volatile _io_reg*)&rg)->bit##bt 

#define F_CPU 3686400
#define BAUD_VALUE 1 // 230.4k @  3.6864MHz

// GPIOR0
#define SERIAL_SENDING GPIOR0, 0
#define WANT_READING GPIOR0, 1
#define WANT_HEADER GPIOR0, 2
#define XMODEM_SENDING GPIOR0, 3
#define XMODEM_FINALIZING GPIOR0, 4
#define XMODEM_NEXT_PACKET GPIOR0, 5
#define READ_FLASH GPIOR0, 6
#define COMMAND_READY GPIOR0, 7

// GPIOR1
#define ACCEL_VERBOSE GPIOR1, 0
#define ACCEL_ENABLED GPIOR1, 1

// Flags
#define flag_serial_sending REGISTER_BIT(SERIAL_SENDING)
#define flag_xmodem_sending REGISTER_BIT(XMODEM_SENDING)
#define flag_xmodem_finalizing REGISTER_BIT(XMODEM_FINALIZING)
#define flag_xmodem_next_packet REGISTER_BIT(XMODEM_NEXT_PACKET)
#define flag_read_flash REGISTER_BIT(READ_FLASH)

#define flag_want_header REGISTER_BIT(WANT_HEADER)

#define flag_command_ready REGISTER_BIT(COMMAND_READY)
#define flag_want_reading REGISTER_BIT(WANT_READING)

#define flag_accel_verbose REGISTER_BIT(ACCEL_VERBOSE)
#define flag_accel_enabled REGISTER_BIT(ACCEL_ENABLED)

// Clock stuff
extern clock_t clock;
extern volatile uint32_t clock_ticks;

// Flash stuff
extern uint16_t flash_addr;
extern uint8_t flash_buf[256];
extern volatile uint16_t flash_buf_ctr;

// Serial stuff
extern char serial_out[132];
extern volatile char serial_in[64];
extern volatile uint8_t serial_in_ctr;
extern volatile uint8_t serial_out_ctr;

#endif
