/* Stepper Motor Second-Hand Clock Demo  */
/* Moves 2× farther each tick */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include "USART.h"

#define TURN 360               /* steps per rotation (choose 360 for clean 6° ticks) */

#define FORWARD    1
#define BACKWARD  -1

#define MAX_DELAY    255
#define MIN_DELAY     10
#define ACCELERATION  16

#define RAMP_STEPS    (MAX_DELAY - MIN_DELAY) / ACCELERATION

// -------- Global Variables --------- //
const uint8_t motorPhases[] = {
  (1 << PB0) | (1 << PB2),   /* full */
  (1 << PB0),                /* half */
  (1 << PB0) | (1 << PB3),
  (1 << PB3),
  (1 << PB1) | (1 << PB3),
  (1 << PB1),
  (1 << PB1) | (1 << PB2),
  (1 << PB2)
};

volatile uint8_t stepPhase = 0;
volatile int8_t direction = FORWARD;
volatile uint16_t stepCounter = 0;

// -------- Functions --------- //
void initTimer(void) {
  TCCR0A |= (1 << WGM01);
  TCCR0B |= (1 << CS00) | (1 << CS02);
  OCR0A = MAX_DELAY;
  sei();
}

ISR(TIMER0_COMPA_vect) {
  stepPhase += direction;
  stepPhase &= 0b00000111;
  PORTB = motorPhases[stepPhase];
  stepCounter++;
}

void takeSteps(uint16_t howManySteps, uint8_t delay) {
  UDR0 = delay;
  OCR0A = delay;
  stepCounter = 0;
  TIMSK0 |= (1 << OCIE0A);

  while (!(stepCounter == howManySteps)) { }

  TIMSK0 &= ~(1 << OCIE0A);
}

void trapezoidMove(int16_t howManySteps) {
  uint8_t delay = MAX_DELAY;
  uint16_t stepsTaken = 0;

  if (howManySteps > 0) direction = FORWARD;
  else {
    direction = BACKWARD;
    howManySteps = -howManySteps;
  }

  if (howManySteps > (RAMP_STEPS * 2)) {

    while (stepsTaken < RAMP_STEPS) {
      takeSteps(1, delay);
      delay -= ACCELERATION;
      stepsTaken++;
    }

    delay = MIN_DELAY;
    takeSteps((howManySteps - 2 * RAMP_STEPS), delay);
    stepsTaken += (howManySteps - 2 * RAMP_STEPS);

    while (stepsTaken < howManySteps) {
      takeSteps(1, delay);
      delay += ACCELERATION;
      stepsTaken++;
    }
  }
  else {
    while (stepsTaken <= howManySteps / 2) {
      takeSteps(1, delay);
      delay -= ACCELERATION;
      stepsTaken++;
    }
    delay += ACCELERATION;
    while (stepsTaken < howManySteps) {
      takeSteps(1, delay);
      delay += ACCELERATION;
      stepsTaken++;
    }
  }
}

// -----------------------------------------------------
//                    MAIN PROGRAM
// -----------------------------------------------------
int main(void) {

  clock_prescale_set(clock_div_1);
  initUSART();
  _delay_ms(500);
  initTimer();

  DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);

  // *** YOUR REQUESTED CHANGE ***
  const uint16_t stepsPerSecond = (TURN / 60) * 13;   // moves twice as far per second

  // -------------- Second-Hand Event Loop --------------- //
  while (1) {
      trapezoidMove(stepsPerSecond);   // larger step movement
      _delay_ms(700);                 // 1-second pause
  }

  return 0;
}
