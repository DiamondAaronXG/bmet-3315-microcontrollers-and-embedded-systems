/*
*This Code starts a one shot timer on buttons press.
*When tthis happens the LED will turn off and turn back on.
*Once the timer counts to a certain value and expires casuing the ISR to fire.
*pulse_active prevents re-triggering until the ISR fires.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED PB2
#define BUTTON PD2

volatile uint8_t pulse_active = 0;

void initializeOneShot(void) {
    // PB2 output
    DDRB |= (1 << LED);
    PORTB &= ~(1 << LED); 

    TCCR1A = 0; 
    TCCR1B = (1 << WGM12);

    OCR1A = 15625;        

    TIMSK1 = (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
    PORTB |= (1 << LED);
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    pulse_active = 0;
}

int main(void) {
    DDRD &= ~(1 << BUTTON);
    PORTD |= (1 << BUTTON);

    initializeOneShot();
    sei(); 

    while (1) {
        if (!pulse_active && bit_is_clear(PIND, BUTTON)) {
            PORTB &= ~(1 << LED); 
            TCNT1 = 0;            
            TCCR1B |= (1 << CS11);
            pulse_active = 1;

            _delay_ms(50);
    }
}
