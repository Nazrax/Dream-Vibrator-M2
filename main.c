#define F_CPU 1000000

#include "main.h"

void init_io(void);

void init_io() {
  DDRB = _BV(DDB2); // Set SS as output
}

int main(int argc, char** argv) {
  init_io();

  return 0;
}
