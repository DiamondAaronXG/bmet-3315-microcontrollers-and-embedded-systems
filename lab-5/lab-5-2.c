/*
*This Code has LED1 blinking
*When each button is pressed the corresponding LED will light up
*without interrupting the flashing of LED1
*
*Added code to have a second LED interact with a second button throughout the
*entirety of the code
*
*/
     /* Demo using pin-change interrupts and in-ISR debouncing routine */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"

#define BUTTON_PIN2 PD3
#define LED_PORT2 PORTB
#define LED_DDR2 DDRB
#define BUTTON_PIN PD2

void initPinChangeInterrupt(void) {
  PCICR |= (1 << PCIE2);        /* set pin-change interrupt for D pins */
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);   /* set mask to look for PCINT18 / PD2 */
  sei();                          /* set (global) interrupt enable bit */
}


ISR(PCINT2_vect) {              /* Run every time button state changes */
  if (bit_is_clear(PIND, BUTTON_PIN)) 
    LED_PORT |= (1 << LED1);
  
  else 
    LED_PORT &= ~(1 << LED1);
  
    if (bit_is_clear(PIND, BUTTON_PIN2)) 
    LED_PORT2 |= (1 << LED2);
  
  else 
    LED_PORT2 &= ~(1 << LED2);
  
}

int main(void) {
  // -------- Inits --------- //
  LED_DDR = 0xff;                                  /* all LEDs active */
	LED_DDR2 = 0xff;
	PORTD |= (1 << BUTTON_PIN) | (1 << BUTTON_PIN2);
  initPinChangeInterrupt();


  // ------ Event loop ------ //
  while (1) {

    _delay_ms(200);
    LED_PORT ^= (1 << LED0);

  }                                                  /* End event loop */
  return 0;                            /* This line is never reached */
}
