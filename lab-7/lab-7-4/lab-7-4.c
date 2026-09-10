/*
*This code causes half of the LED array to be on and the other off.
*It alternates through smooth blinking off each half simultaneously.
*
*Added code to produce a PWM waveform on pin 25.
*
*No arbitrary pin connections
*/
// Quick and dirty demo of how to get PWM on any pin with interrupts
// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"

#define DELAY 8   // Adjusted to get ~0.50 Hz cycle frequency

volatile uint8_t brightnessA;
volatile uint8_t brightnessB;

// *** Added for PWM pin 25 (PC2) ***
#define PWM_DDR DDRC
#define PWM_PORT PORTC
#define PWM_PIN PC2     // Pin 25 on ATmega328

// -------- Functions --------- //
static inline void initTimer0(void) {
                                 /* must be /64 or more for ISR timing */
  TCCR0B |= (1 << CS01) | (1 << CS00);
                                     /* both output compare interrupts */
  TIMSK0 |= ((1 << OCIE0A) | (1 << OCIE1B));
  TIMSK0 |= (1 << TOIE0);                 /* overflow interrupt enable */
  sei();
}

ISR(TIMER0_OVF_vect) {
  LED_PORT = 0xff;

  // *** Turn PWM pin ON at start of PWM cycle ***
  PWM_PORT |= (1 << PWM_PIN);

  OCR0A = brightnessA;
  OCR0B = brightnessB;
}

ISR(TIMER0_COMPA_vect) {
  LED_PORT &= 0b11110000;                    /* turn off low four LEDs */

  // *** Turn PWM pin OFF when duty expires ***
  PWM_PORT &= ~(1 << PWM_PIN);
}

ISR(TIMER0_COMPB_vect) {
  LED_PORT &= 0b00001111;                   /* turn off high four LEDs */
}

int main(void) {
  // -------- Inits --------- //

  uint8_t i;
  LED_DDR = 0xff;
  initTimer0();

  // *** Make pin 25 (PC2) an output ***
  PWM_DDR |= (1 << PWM_PIN);

  // ------ Event loop ------ //
  while (1) {

    // Ramp up duty cycle smoothly
    for (i = 0; i < 255; i++) {
      _delay_ms(DELAY);
      brightnessA = i;
      brightnessB = 255 - i;
    }

    // Reset duty cycle (no ramp down)
    brightnessA = 0;
    brightnessB = 255;

  }                                                  /* End event loop */
  return 0;                            /* This line is never reached */
}
