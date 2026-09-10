/*
*This code causes the LED bar graph to show 1 bar after another in a left to 
*right to left pattern.
*
*Changed DELAYTIME variable defined in line 16 to a value of "39" 
*(Not part of lab explicitely)
*
*PB0-PB7 connected to pins 1-8 on the LED bar
*/
                                                         /* Cylon Eyes */

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */

#define DELAYTIME 39                                  /* milliseconds */
#define LED_PORT                PORTB
#define LED_PIN                 PINB
#define LED_DDR                 DDRB

int main(void) {

  // -------- Inits --------- //
  uint8_t i=0;
  LED_DDR = 0xff;               /* Data Direction Register B:
                                   all set up for output */

  // ------ Event loop ------ //
  while (1) {

    while (i < 7) {
      LED_PORT = (1 << i);                 /* illuminate only i'th pin */
      _delay_ms(DELAYTIME);                                    /* wait */
      i = i + 1;                               /* move to the next LED */
    }

    while (i > 0) {
      LED_PORT = (1 << i);                 /* illuminate only i'th pin */
      _delay_ms(DELAYTIME);                                    /* wait */
      i = i - 1;                           /* move to the previous LED */
    }

  }                                                  /* End event loop */
  return 0;
}
