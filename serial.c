#include "globals.h"
#include "serial.h"

#include <avr/interrupt.h>
#include <util/delay.h>

volatile char serial_in[64];
char serial_out[132];
volatile uint8_t serial_in_ctr;
volatile uint8_t serial_out_ctr;

inline void usart_init(void) {
  UBRR0 = BAUD_VALUE;
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01); // 8-bit, 1 stop bit, no parity
  UCSR0B = _BV(TXEN0) | _BV(TXCIE0) | _BV(RXEN0) | _BV(RXCIE0); // Turn on TX and RX and their interrupts
  UCSR0A = _BV(U2X0); // Double serial clock
}

void usart_send_next_byte(void) {
  bool_t length_ok = serial_out_ctr < 132;
  bool_t notnull = serial_out[serial_out_ctr] != '\0';


  if (length_ok && notnull) {

    while (!(UCSR0A & _BV(UDRE0))); // Ensure empty transmit buffer
    UDR0 = serial_out[serial_out_ctr];
    serial_out_ctr++;
  } else {
    flag_serial_sending = false;
  }
}

void usart_dostuff() {

}

void usart_reset_output(void) {
  int i;
  for(i=0; i<132; i++)
    serial_out[i] = 0;
}

ISR(USART_TX_vect) {
  usart_send_next_byte();
}

ISR(USART_RX_vect) {
  // Ignore it for now
}
