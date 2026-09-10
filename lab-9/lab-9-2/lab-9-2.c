// Dual-PWM smooth motor control for H-bridge (PD6 = OC0A, PD5 = OC0B)
// Compiles for AVR (e.g. ATmega328P)
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"   // keep if you use LED_PORT, LED_DDR, LED0, LED1

// --- Utility: safe variable delay (AVR _delay_ms needs compile-time const) --- //
void delayVar(uint8_t ms) {
    for (uint8_t i = 0; i < ms; i++) {
        _delay_ms(1);
    }
}

// --- PWM init: both OC0A (PD6) and OC0B (PD5) --- //
void initPWM_both(void) {
    // Make PD6 and PD5 outputs (OC0A and OC0B)
    DDRD |= (1 << PD6) | (1 << PD5);

    // Fast PWM (WGM01 and WGM00), non-inverting on both OC0A and OC0B
    // COM0A1 = 1: non-inverting OC0A, COM0B1 = 1: non-inverting OC0B
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);

    // Prescaler = 1 -> fastest PWM (~62.5kHz timer clock => ~62.5kHz / 2? practical ~31kHz switching)
    TCCR0B = (1 << CS00);

    // Start with 0 duty on both outputs
    OCR0A = 0;
    OCR0B = 0;
}

// --- Set speeds for each side (0..255) --- //
static inline void setSpeedA(uint8_t v) { OCR0A = v; }  // OC0A (PD6)
static inline void setSpeedB(uint8_t v) { OCR0B = v; }  // OC0B (PD5)

// --- Ramp helpers: ramp the active PWM from 0 -> max and back --- //
void rampUpA(uint8_t maxSpeed, uint8_t stepDelay) {
    for (uint16_t i = 0; i <= maxSpeed; i++) {    // use uint16_t to avoid wrap
        setSpeedA((uint8_t)i);
        delayVar(stepDelay);
    }
}
void rampDownA(uint8_t maxSpeed, uint8_t stepDelay) {
    for (int16_t i = maxSpeed; i >= 0; i--) {
        setSpeedA((uint8_t)i);
        delayVar(stepDelay);
    }
}
void rampUpB(uint8_t maxSpeed, uint8_t stepDelay) {
    for (uint16_t i = 0; i <= maxSpeed; i++) {
        setSpeedB((uint8_t)i);
        delayVar(stepDelay);
    }
}
void rampDownB(uint8_t maxSpeed, uint8_t stepDelay) {
    for (int16_t i = maxSpeed; i >= 0; i--) {
        setSpeedB((uint8_t)i);
        delayVar(stepDelay);
    }
}

// --- LED helpers (optional) --- //
// If your LED pins conflict with PD5/PD6, move LED defines in pinDefines.h accordingly.
// Here we assume LED0 and LED1 are NOT PD5/PD6. If they are, remove these writes.
static inline void ledForwardOn(void)  { LED_PORT |=  (1 << LED0); }
static inline void ledForwardOff(void) { LED_PORT &= ~(1 << LED0); }
static inline void ledReverseOn(void)  { LED_PORT |=  (1 << LED1); }
static inline void ledReverseOff(void) { LED_PORT &= ~(1 << LED1); }

int main(void) {
    LED_DDR |= (1 << LED0) | (1 << LED1);
    LED_PORT &= ~((1 << LED0) | (1 << LED1));

    initPWM_both();

    uint8_t maxSpeed = 200;
    uint8_t rampTime = 1;   // smaller = faster transitions

    while (1) {
        // --------- FORWARD ---------
        ledReverseOff();
        ledForwardOn();
        setSpeedB(0);               // ensure other side inactive
        rampUpA(maxSpeed, rampTime);
        _delay_ms(800);             // hold speed (you can remove this too)

        // --- transition to reverse: NO STOP, NO DEADTIME ---
        rampDownA(maxSpeed, rampTime);  // ramp down forward
        setSpeedA(0);                    // immediately switch
        rampUpB(maxSpeed, rampTime);     // ramp up reverse

        // --------- REVERSE ---------
        ledForwardOff();
        ledReverseOn();
        _delay_ms(800);

        // --- transition to forward ---
        rampDownB(maxSpeed, rampTime);
        setSpeedB(0);
        rampUpA(maxSpeed, rampTime);
    }

    return 0;
}
