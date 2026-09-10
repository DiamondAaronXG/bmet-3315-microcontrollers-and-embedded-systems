/*
*This code reads an 8-bit ADC value from ADC0 every 500ms 
*it then sends it over USART
*/
#include <avr/io.h>
#include <util/delay.h>

// -------- USART Definitions --------- //
#define MYUBRR ((F_CPU/16/9600UL)-1)   // BAUD = 9600

// -------- Function Prototypes --------- //
static inline void initADC0(void);
uint8_t analogRead(void);
void initMyUSART(void);
void myTransmitByte(uint8_t data);
void myPrintWord(uint16_t word);

// -------- Functions --------- //
static inline void initADC0(void) {
    ADMUX = (1 << REFS0) | (1 << ADLAR);   // AVCC ref, LEFT-adjust ADC result
    ADCSRA = (1 << ADPS2) | (1 << ADEN);   // prescaler /16, enable ADC
}

void initMyUSART(void) {
    UBRR0H = (uint8_t)(MYUBRR >> 8);   // fix warning with cast
    UBRR0L = (uint8_t)MYUBRR;
    UCSR0B = (1 << TXEN0);              // enable transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

void myTransmitByte(uint8_t data) {
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = data;
}

void myPrintWord(uint16_t word) {
    char buffer[6];
    uint8_t i = 0;

    if (word == 0) {
        myTransmitByte('0');
    } else {
        while (word > 0) {
            buffer[i++] = '0' + (word % 10);
            word /= 10;
        }
        while (i > 0) {
            myTransmitByte(buffer[--i]);
        }
    }
    myTransmitByte('\n');
}

// -------- ADC Read: ONLY MOST SIGNIFICANT 8 BITS -------- //
uint8_t analogRead(void) {
    ADMUX = (ADMUX & 0xF0) | 0x00;       // Select ADC0
    ADCSRA |= (1 << ADSC);               // Start conversion
    loop_until_bit_is_clear(ADCSRA, ADSC);
    return ADCH;                         // Return MSB 8 bits only
}

// -------- Main --------- //
int main(void) {
    uint8_t adcValue8;

    initADC0();
    initMyUSART();

    while (1) {
        adcValue8 = analogRead();   // 0–255
        myPrintWord(adcValue8);     // send over USART

        _delay_ms(500);
    }

    return 0;
}
