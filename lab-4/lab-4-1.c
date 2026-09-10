#include <avr/io.h>
#include <util/delay.h>

// -------- Pin Definitions --------- //
// These would normally be in "pinDefines.h"
#define LED_DDR   DDRB
#define LED_PORT  PORTB

// -------- USART Definitions --------- //
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

// -------- Function Prototypes --------- //
static inline void initADC0(void);
uint16_t analogRead(void);
uint8_t linearScale(uint16_t Din);
void displayBarGraph(uint8_t value);
void initUSART(void);
void transmitByte(uint8_t data);
void printWord(uint16_t word);

// -------- Functions --------- //
static inline void initADC0(void) {
  ADMUX |= (1 << REFS0);                  /* reference voltage on AVCC /
  ADCSRA |= (1 << ADPS2);                 / ADC clock prescaler /16 /
  ADCSRA |= (1 << ADEN);                  / enable ADC */
}
void initUSART(void) {
  UBRR0H = (MYUBRR >> 8);
  UBRR0L = MYUBRR;
  UCSR0B = (1 << TXEN0);                  /* Enable transmitter /
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); / 8-bit data /
}

void transmitByte(uint8_t data) {
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = data;
}

void printWord(uint16_t word) {
  char buffer[6];
  uint8_t i = 0;
  if (word == 0) {
    transmitByte('0');
  } else {
    while (word > 0) {
      buffer[i++] = '0' + (word % 10);
      word /= 10;
    }
    while (i > 0) {
      transmitByte(buffer[--i]);
    }
  }
  transmitByte('\n');
}

uint16_t analogRead(void) {
  ADMUX = (ADMUX & 0xF0) | 0x00;          // Select ADC0 (PC0) as input channel
  ADCSRA |= (1 << ADSC);                  // Start ADC conversion
  loop_until_bit_is_clear(ADCSRA, ADSC);  // Wait until done
  return ADC;                             // Read ADC value
}

uint8_t linearScale(uint16_t Din) {
  return (Din >> 7);                      / scale 10-bit to 3-bit */
}

void displayBarGraph(uint8_t value) {
  LED_PORT = 0;
  for (uint8_t i = 0; i <= value; i++) {
    LED_PORT |= (1 << i);
  }
}

// -------- Main --------- //
int main(void) {
  uint8_t ledValue;
  uint16_t adcValue;

  initADC0();
  initUSART();
  LED_DDR = 0xff;

  while (1) {
    adcValue = analogRead();
    ledValue = linearScale(adcValue);
    displayBarGraph(ledValue);
    printWord(adcValue); // Send measured value to serial monitor
    _delay_ms(500);
  }
  return 0;
}
