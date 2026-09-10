/*
*This code causes the LED bar graph to show 1 bar after another in a left to 
*right to left patter.
*
*Changed DELAYTIME variable defined in line 21 to a value of "500"
*(Not part of lab explicitely)
*
*PB0-PB7 connected to pins 1-8 on the LED bar
*/
/*

Cylon eyes actually turns out to be a decent POV routine.
The only change here is in DELAYTIME.

 */


#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */

#define DELAYTIME 500                                   /* milliseconds */

int main(void) {
  uint8_t i=0;
  DDRB = 0b11111111;              /* Data Direction Register B: all on */

  while (1) {

    while (i < 7) {
      PORTB = (1 << i);                    /* illuminate only i'th pin */
      _delay_ms(DELAYTIME);                                    /* wait */
      i = i + 1;                               /* move to the next LED */
    }

    while (i > 0) {
      PORTB = (1 << i);                    /* illuminate only i'th pin */
      _delay_ms(DELAYTIME);                                    /* wait */
      i = i - 1;                           /* move to the previous LED */
    }
  }

  return 0;
}
