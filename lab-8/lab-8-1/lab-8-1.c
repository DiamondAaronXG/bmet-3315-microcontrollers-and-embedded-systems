/*
*This code generates a sine wave audio tone using DDS and PWM
*it only outputs when the button is held
*while providing a debug timing pin toggle
*/
/* Direct-digital synthesis — Refactored */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

#include "pinDefines.h"
#include "fullSine.h"

#define TEST_PIN PD5
#define S_DELAY 50     // microsecond delay


/* ---------------- Hardware Functions ---------------- */

// ---- TIMER0 Hardware ----
void initTimer0(void) {
    TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B = (1 << CS00);        // No prescaler
}

// ---- SPEAKER Pin ----
void enableSpeaker(void) {
    SPEAKER_DDR |= (1 << SPEAKER);
}

// ---- PWM Output ----
void updatePWM(int8_t value) {
    loop_until_bit_is_set(TIFR0, TOV0);
    OCR0A = 128 + value;
    TIFR0 |= (1 << TOV0);    // Clear overflow
    _delay_us(S_DELAY);
}

// ---- BUTTON Hardware ----
void initButton(void) {
    BUTTON_PORT |= (1 << BUTTON);   // Pull-up
}

uint8_t isButtonPressed(void) {
    return bit_is_clear(BUTTON_PIN, BUTTON);
}

// ---- TEST PIN Hardware ----
void initTestPin(void) {
    DDRD |= (1 << TEST_PIN);
}

void toggleTestPin(void) {
    PORTD ^= (1 << TEST_PIN);
}


/* ---------------- Main Application Logic ---------------- */

int main(void) {

    uint16_t accumulator = 0;
    uint16_t accumulatorSteps = 880;     // ~440Hz
    uint8_t waveStep;
    int8_t pwmValue;

    // ---- Inits ----
    clock_prescale_set(clock_div_1);
    initTimer0();
    initButton();
    initTestPin();

    // ---- Main Loop ----
    while (1) {

        if (isButtonPressed()) {

            enableSpeaker();

            accumulator += accumulatorSteps;
            waveStep = accumulator >> 8;
            pwmValue = fullSine[waveStep];

            updatePWM(pwmValue);

            _delay_ms(50);
        }

        toggleTestPin();   // preserves your timing debug pin
    }

    return 0;
}
