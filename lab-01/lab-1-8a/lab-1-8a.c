                                                       /* Blinker Demo */
// Aaron Grewal lab-1-8a Sept. 09 2025
// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */


int main(void) {

  // -------- Inits --------- //
  DDRB |= 0b00100000; // Changed "00000001" to "00100000" to change the "flashing pin"
           /* Data Direction Register B:
                                   writing a one to the bit
                                   enables output. */

  // ------ Event loop ------ //
  while (1) {

    PORTB = 0b00100000; // Same as above change
             /* Turn on first LED bit/pin in PORTB */
    _delay_ms(1000);                                           /* wait */

    PORTB = 0b00000000; // Same as above change 
            /* Turn off all B pins, including LED */
    _delay_ms(1000);                                           /* wait */

  }                                                  /* End event loop */
  return 0;                            /* This line is never reached */
}
