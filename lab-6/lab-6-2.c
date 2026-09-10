/*
*This Code plays a predetermined musical note for a predetermined time each
*time the button connected to PD2 is pressed.
*After the last note, the first note is played again
*
*/
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "scale8.h"  

#define BUTTON_INPUT   PIND
#define BUTTON_PIN PD2
#define BUTTON_PORT PORTD
#define BUTTON_DDR DDRD



static inline void initTimer(void) {
    TCCR0A |= (1 << WGM01);   // CTC mode
    TCCR0A |= (1 << COM0A0);  // Toggle OC0A on compare
    TCCR0B |= (1 << CS01) | (1 << CS00); // clk/64 prescaler
}

static inline void playNote(uint8_t period, uint16_t duration) {
    TCNT0 = 0;
    OCR0A = period;
    SPEAKER_DDR |= (1 << SPEAKER);

    while (duration--) {
        _delay_ms(1);
    }

    SPEAKER_DDR &= ~(1 << SPEAKER); // turn speaker off
}

int main(void) {
    initTimer();
    BUTTON_PORT |= (1 << BUTTON_PIN); // enable pull-up
    BUTTON_DDR &= ~(1 << BUTTON_PIN);

    uint8_t counter = 0;
    uint8_t lastButtonState = 1;

    const uint8_t soundTable[8] = { C2, D2, E2, F2, G2, A2, B2, C3 };
    const uint16_t durationTable[8] = { 200, 200, 200, 200, 200, 200, 200, 800 };

    while (1) {
        uint8_t currentButtonState = (BUTTON_INPUT & (1 << BUTTON_PIN)) >> BUTTON_PIN;

        // detect press
        if (lastButtonState == 1 && currentButtonState == 0) {
            _delay_ms(20); // debounce

            if ((BUTTON_INPUT & (1 << BUTTON_PIN)) == 0) {
                counter++;
                if (counter >= 8) counter = 0;
                playNote(soundTable[counter], durationTable[counter]);
            }

            // wait for release
            while ((BUTTON_INPUT & (1 << BUTTON_PIN)) == 0) _delay_ms(10);
        }

        lastButtonState = currentButtonState;
    }

    return 0;
}
