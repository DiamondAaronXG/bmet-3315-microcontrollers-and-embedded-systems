/*
*This code sends inputted text through serial to the ATMEGA328P.
*It will then light up an LED bar corrosponding to the binary signal.
*
*Added the "initUSART()" function starting line 22.
*
*No arbitrary pin connections.
*/

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"

int main(void) {
  char serialCharacter;

/*
*Enables rx and tx, 7 data bits,even parity, 1 stop bit
*/
	initUSART();
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    	UCSR0B &= ~(1 << UCSZ02);
    	UCSR0C |= (1 << UCSZ01);
    	UCSR0C &= ~(1 << UCSZ00);
    	UCSR0C |= (1 << UPM01);
    	UCSR0C &= ~(1 << UPM00);
    	UCSR0C &= ~(1 << USBS0);

  // -------- Inits --------- //
  LED_DDR = 0xff;                            /* set up LEDs for output */
  printString("Hello World!\r\n");                          /* to test */

  // ------ Event loop ------ //
  while (1) {

    serialCharacter = receiveByte();
    transmitByte(serialCharacter);
    LED_PORT = serialCharacter;
                           /* display ascii/numeric value of character */

  }                                                  /* End event loop */
  return 0;
}
