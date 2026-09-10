/*
*This Code increments a 7-seg display from 0-F at 2Hz
*Whenever the button is pressed the decimal point will light up
*without interuppting the incrementing of the 7-seg display
*
*Added digitTable and code to integrate the table with the rest of the code
*
*/
/*

Demo of using interrupts for doing what they do best --
two things at once.

Flashes LED0 at a fixed rate, interrupting whenever button is pressed.

 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"
#include <avr/power.h>

#define BUTTON_PIN PD2
#define BUTTON_PORT PORTD
#define BUTTON_INPUT PIND
#define SEG_DDR DDRB
#define SEG_PORT PORTB

// Common anode digit table (0=segment on, 1=segment off)
const uint8_t digitTable[16] = {
	0b00100000, // 0
	0b11101011, // 1
	0b01001100, // 2
	0b01001001, // 3
	0b10001011, // 4
	0b00011001, // 5
	0b00011000, // 6
	0b01101011, // 7
	0b00001000, // 8
	0b00001001, // 9
	0b00001010, // A
	0b10011000, // b
	0b00111100, // C
	0b11001000, // d
	0b00011100, // E
	0b00011110, // F
};

ISR(INT0_vect) {         /* Run every time there is a change on button */
  if (bit_is_clear(PIND, PD2)) {
    LED_PORT |= (1 << LED1);
  }
  else {
    LED_PORT &= ~(1 << LED1);
  }
}

void initInterrupt0(void) {
  EIMSK |= (1 << INT0);                                 /* enable INT0 */
  EICRA |= (1 << ISC01);                /* trigger when button changes */
  sei();                          /* set (global) interrupt enable bit */
}

int main(void) {
  // -------- Inits --------- //
	int counter = 0;
  LED_DDR = 0xff; 
	LED_PORT = 0x00;                                  /* all LEDs active */
  BUTTON_PORT |= (1 << BUTTON_PIN);                              /* pullup */
  	SEG_DDR = 0xFF;
  	SEG_PORT = 0xFF;
  initInterrupt0();
  	int segments = 0;

  // ------ Event loop ------ //
  	while(1) {
  		counter++;
  		if (counter > 15)
  			counter = 0;

		segments = (digitTable[counter]);

		uint8_t decimalBit = (BUTTON_INPUT & (1 << BUTTON_PIN)) ? (1 << PB3) : 0;

        	// --- Prepare segments ---
        	// Mask out PB3 from digitTable and OR in decimalBit
        	uint8_t segments = (digitTable[counter] & ~(1 << PB3)) | decimalBit;

        	// --- Update display ---
        	SEG_PORT = segments;  // atomic write to PORTB
  		_delay_ms(500);
  	}                                              /* End event loop */
 	 return 0;                            /* This line is never reached */
}
