// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h" /added USART library/

// -------- Function Prototypes -------- //
static inline void initADC8(void);
uint16_t analogRead(void);
uint8_t tempToLED(uint16_t Din);
void bargraphdisp(uint8_t leddisplay);

// -------- Functions --------- //
static inline void initADC8(void) {
  // Internal 1.1V reference, MUX3 selects temperature sensor (ADC8)
  ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << MUX3);
  ADCSRB = 0;                               // Free running mode off
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // enable ADC, prescaler /128
  _delay_ms(1);                             // allow reference to stabilize
}

uint16_t analogRead(void) {
  ADCSRA |= (1 << ADSC);                    // start ADC conversion
  loop_until_bit_is_clear(ADCSRA, ADSC);    // wait until done
  return ADC;                               // return ADC result
}

uint8_t tempToLED(uint16_t Din) {
  // Convert ADC value to LED pattern (adjust thresholds for your chip)
  if (Din < 340) return 0;       // Cool → 1 LED
  else if (Din < 345) return 1;
  else if (Din < 355) return 2;
  else if (Din < 360) return 3;
  else if (Din < 375) return 4;
  else if (Din < 385) return 5;
  else if (Din < 400) return 6;
  else return 7;                 // Hot → all LEDs
}

void bargraphdisp(uint8_t leddisplay) {
  LED_PORT = 0;
  for (uint8_t i = 0; i <= leddisplay; i++) {
    LED_PORT |= (1 << i);        // Light up LEDs up to threshold
  }
}
int main(void) {
  uint8_t ledValue, prevledValue = 255;
  uint16_t adcValue;

  initUSART();
  initADC8();
  LED_DDR = 0xff;
  printString("Internal Temperature Sensor Active\r\n");

  // ------ Event loop ------ //
  while (1) {
    adcValue = analogRead();
    ledValue = tempToLED(adcValue);
    bargraphdisp(ledValue);

    if (ledValue != prevledValue) {
      printString("Temperature ADC: ");
      printWord(adcValue);
      printString(" | LED Level: ");
      printByte(ledValue);
      printString("\r\n");
      prevledValue = ledValue;
    }

    _delay_ms(500);
  }

  return 0;
}
