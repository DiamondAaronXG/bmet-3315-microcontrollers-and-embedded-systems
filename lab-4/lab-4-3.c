/*
*This code measures and returns the value of ADC Band Gap & VREF through serial
*/
// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"

// Function prototypes
static inline void initADC_Bandgap(void);
uint16_t analogRead(void);
uint8_t scaleVrefToBar(uint16_t adcValue);
void displayBarGraph(uint8_t LEDValue);

// -------- Functions --------- //
static inline void initADC_Bandgap(void) {
    // External AREF as reference, measure internal 1.1V bandgap
    ADMUX = (1 << MUX3) | (1 << MUX2) | (1 << MUX1);
    ADCSRB = 0;
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128
    _delay_ms(2);
}

uint16_t analogRead(void) {
    ADCSRA |= (1 << ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    return ADC;
}

// Convert ADC bandgap reading into VREF (mV), then scale to 0–7 LEDs
uint8_t scaleVrefToBar(uint16_t adcValue) {
    // Vref (mV) ≈ (1.1 * 1023 * 1000) / adcValue
    uint32_t vrefMilliVolts = 1125300UL / adcValue;

    // range 1.1V–5.0V 
    if (vrefMilliVolts < 1100) vrefMilliVolts = 1100;
    if (vrefMilliVolts > 5000) vrefMilliVolts = 5000;

    // Scale: 1100–5000 mV 
    return (uint8_t)(((vrefMilliVolts - 1100) * 8) / (5000 - 1100));
}

void displayBarGraph(uint8_t LEDValue) {
    uint8_t i;
    LED_PORT = 0;
    for (i = 0; i <= LEDValue; i++) {
        LED_PORT |= (1 << i);
    }
};
//Forwarded
int main(void) {
    uint16_t adcValue;
    uint8_t LEDValue;
    uint32_t vrefMilliVolts;

    initADC_Bandgap();
    initUSART();
    LED_DDR = 0xFF;

    printString("Verifying VREF with LED bar...\r\n");

    while (1) {
        adcValue = analogRead();
        vrefMilliVolts = 1125300UL / adcValue;
        LEDValue = scaleVrefToBar(adcValue);

        displayBarGraph(LEDValue);


        printString("ADC Band Gap: ");
        printWord(adcValue);
 printString("\tVREF ≈ ");
        printWord(vrefMilliVolts);
        printString(" mV\r\n");


        _delay_ms(1000);
    }
}
