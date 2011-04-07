#include "globals.h"
#include "commands.h"
#include "serial.h"
#include "flash.h"
#include "accel.h"

#include <avr/io.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <string.h>

const char on[] PROGMEM = ": on";
const char off[] PROGMEM = ": off";
const char prompt[] PROGMEM = "\r\n> ";
const char badinput[] PROGMEM = "\r\nBad input";
const char executed[] PROGMEM = "\r\nCommand executed: ";
const char cmdresult[] PROGMEM = "\r\nResult:";
const char youentered[] PROGMEM = "\r\nYou entered:";
const char timeisnow[] PROGMEM = "\r\nThe time is now: ";
const char timepattern[] PROGMEM = "%02d:%02d:%02d";

static char* p;

static void handle_time(void);
static void handle_accelverb(void);
static void handle_accel(void);
static void handle_spi(void);

void handle_command() {
  serial_in[serial_in_ctr] = '\0';
  p = serial_out;

  if (!strncmp_P((char*)serial_in, PSTR("TIME"), 4)) {
    handle_time();
  } else if (!strncmp_P((char*)serial_in, PSTR("ACCELVERB"), 9)) {
    handle_accelverb();
  } else if (!strncmp_P((char*)serial_in, PSTR("ACCEL"), 5)) {
    handle_accel();
    p += strlcpy_P(p, off, 64);
  } else if (!strncmp_P((char*)serial_in, PSTR("SCAN"), 4)) {
    flash_scan();
    p += strlcpy_P(p, cmdresult, 64);
    p += sprintf(p, " %03d %03d", flash_addr >> 8, flash_addr & 0xFF);
  } else if (!strncmp_P((char*)serial_in, PSTR("ADDR"), 4)) {
    p += strlcpy_P(p, cmdresult, 64);
    p += sprintf(p, " %03d %03d", flash_addr >> 8, flash_addr & 0xFF);
  } else if (!strncmp_P((char*)serial_in, PSTR("ERASE"), 5)) {
    flash_erase();
    p += strlcpy_P(p, executed, 64);
    p += strlcpy_P(p, PSTR(" Erase"), 64);
  } else if (!strncmp_P((char*)serial_in, PSTR("SPI"), 3)) {
    handle_spi();
  } else if (!strncmp_P((char*)serial_in, PSTR("SELA"), 4)) {
    accel_select();
    p += strlcpy_P(p, cmdresult, 64);
    p += strlcpy_P(p, PSTR("Accel Selected"), 64);
  } else if (!strncmp_P((char*)serial_in, PSTR("SELF"), 4)) {
    flash_select();
    p += strlcpy_P(p, cmdresult, 64);
    p += strlcpy_P(p, PSTR("Flash Selected"), 64);
  } else if (!strncmp_P((char*)serial_in, PSTR("DSELA"), 5)) {
    accel_deselect();
    p += strlcpy_P(p, cmdresult, 64);
    p += strlcpy_P(p, PSTR("Accel Deselected"), 64);
  } else if (!strncmp_P((char*)serial_in, PSTR("DSELF"), 5)) {
    flash_deselect();
    p += strlcpy_P(p, cmdresult, 64);
    p += strlcpy_P(p, PSTR("Flash Deselected"), 64);
  } else if (strlen((char*)serial_in) == 0) {
    // Do nothing
  } else {
    p += strlcpy_P(p, PSTR("\r\nUnknown: "), 64);
    strncpy(p, (char*)serial_in, serial_in_ctr);
    p += serial_in_ctr;
  } 

  strcpy_P(p, prompt);
  usart_send();

  serial_in_ctr = 0;
  flag_command_ready = false;
}

static void handle_time() {
  if (serial_in_ctr == 4) {
    uint8_t timepatlen = strlen_P(timepattern);
    char pattern[timepatlen+1];
    strcpy_P(pattern, timepattern);
    
    p += strlcpy_P(p, timeisnow, 64);
    p += sprintf(p, pattern, clock.hours, clock.minutes, clock.seconds);
  } else if (serial_in_ctr == 10) {
    clock.hours = (serial_in[4] - '0') * 10 + (serial_in[5] - '0');
    clock.minutes = (serial_in[6] - '0') * 10 + (serial_in[7] - '0');
    clock.seconds = (serial_in[8] - '0') * 10 + (serial_in[9] - '0');
 
    p += strlcpy_P(p, executed, 64);
    p += strlcpy_P(p, PSTR("TIME"), 64);
  } else {
    p += strlcpy_P(p, badinput, 64);
  }
}

static void handle_spi() {
  int i;

  strlcpy_P(p, cmdresult, 64);
  for(i=1; (i*3)+2<serial_in_ctr; i++) {
    uint8_t j = i * 3;
    uint8_t x = (serial_in[j] - '0') * 100 + (serial_in[j+1] - '0') * 10 + (serial_in[j+2]-'0');
    uint8_t result = spi_send(x);
      
    p += sprintf(p, "{%03d : %03d} ", x, result);
  }
}


static void handle_accelverb() {
  flag_accel_verbose = !flag_accel_verbose;
  p += strlcpy_P(p, executed, 64);
  p += strlcpy_P(p, PSTR("ACCEL verbose"), 64);
  if (flag_accel_verbose)
    p += strlcpy_P(p, on, 64);
  else
    p += strlcpy_P(p, off, 64);
}

static void handle_accel() {
  flag_accel_enabled = !flag_accel_enabled;
  p += strlcpy_P(p, executed, 64);
  p += strlcpy_P(p, PSTR("ACCEL"), 64);
  if (flag_accel_enabled)
    p += strlcpy_P(p, on, 64);
  else
    p += strlcpy_P(p, off, 64);
}
