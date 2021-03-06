#ifndef _TYPES_H_
#define _TYPES_H_

typedef enum {false, true} bool_t;
typedef enum { DOWN, UP } button_state_t;

typedef struct {
  unsigned int subseconds:10;
  unsigned char seconds:6;
  unsigned char minutes:6;
  unsigned char hours:5;
} clock_t;

typedef struct
{
  unsigned int bit0:1;
  unsigned int bit1:1;
  unsigned int bit2:1;
  unsigned int bit3:1;
  unsigned int bit4:1;
  unsigned int bit5:1;
  unsigned int bit6:1;
  unsigned int bit7:1;
} _io_reg; 

#endif
