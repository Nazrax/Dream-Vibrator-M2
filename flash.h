#ifndef _FLASH_H_
#define _FLASH_H_

#include <avr/io.h>

#include "globals.h"
#include "spi.h"
#include "types.h"

#define INSTR_POWERDOWN 0xB9
#define INSTR_POWERUP 0xAB

#define INSTR_READ_DATA 0x03

#define INSTR_WRITE_ENABLE 0x06
#define INSTR_PAGE_PROGRAM 0x02
#define INSTR_CHIP_ERASE 0x0C7

#define INSTR_READ_STATUS1 0x05
#define INSTR_READ_STATUS2 0x35

#define STATUS_BUSY 0x01
#define STATUS_WEL  0x02

#define flash_select() PORTD &= ~(_BV(PORTD3))
#define flash_deselect() PORTD |= _BV(PORTD3)

#define flash_powerdown() flash_select();spi_send(INSTR_POWERDOWN);flash_deselect();
#define flash_powerup() flash_select();spi_send(INSTR_POWERUP);flash_deselect();

void flash_init(void);
void flash_wait_for_idle(void);
void flash_erase(void);
void flash_write(uint16_t);
void flash_read(uint16_t);
bool_t flash_busy(void);
void flash_scan(void);
uint8_t flash_status(void);

#endif // _FLASH_H_
