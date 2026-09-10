/*
*This code samples ADC0 at 1kHz using a Timer1 interrupt
*sends each 8-bit reading directly to a Timer0 PWM output
*while toggling a test pin for timing.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// -------- Pin Definitions --------- //
#define PWM_DDR    DDRB
#define PWM_PORT   PORTB
#define PWM_PIN    PB1      // OC1A or OC0A (we'll use Timer0 PWM)
#define TEST_DDR   DDRD
#define TEST_PORT  PORTD
#define TEST_PIN   PD5

// -------- ADC --------- //
static inline void initADC0(void) {
    ADMUX = (1 << REFS0) | (1 << ADLAR);  // AVCC ref, LEFT-adjusted ADC
    ADCSRA = (1 << ADPS2) | (1 << ADEN);  // prescaler /16, enable ADC
}

static inline uint8_t analogRead8(void) {
    ADMUX = (ADMUX & 0xF0) | 0x00;        // select ADC0
    ADCSRA |= (1 << ADSC);                // start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    return ADCH;                           // return MSB 8 bits only
}

// -------- PWM (Timer0 Fast PWM) --------- //
static inline void initPWM(void) {
    PWM_DDR |= (1 << PWM_PIN);            // output pin
    TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01); // fast PWM
    TCCR0B = (1 << CS00);                 // no prescaler
}

// -------- Timer1 for 1 kHz sampling --------- //
static inline void initTimer1(void) {
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // CTC, prescaler = 64
    OCR1A =3;  // (F_CPU/64/1000Hz)-1 = (1MHz/64/1000)-1 ≈ 15
    TIMSK1 = (1 << OCIE1A);  // enable compare match interrupt
}

// -------- Main --------- //
int main(void) {
    // ---- Init hardware ----
    initADC0();
    initPWM();
    TEST_DDR |= (1 << TEST_PIN);  // test pin

    initTimer1();
    sei();  // enable global interrupts

    while (1) {
        // main loop empty; all work happens in ISR
    }

    return 0;
}

// -------- ISR: triggered at 1 kHz --------- //
ISR(TIMER1_COMPA_vect) {
    uint8_t adcValue = analogRead8();  // sample ADC
    OCR0A = adcValue;                  // output to PWM
    TEST_PORT ^= (1 << TEST_PIN);      // toggle test pin for timing verification
}
