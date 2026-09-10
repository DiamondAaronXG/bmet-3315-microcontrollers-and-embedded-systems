/*
*This code counts using a simple NO button
*Each button press will increment a 7 segment display from 1-9 then A-F
*
*Changed code from lab-2-6 to allow for a push button to be used
*Allows for a 7 segment display with common anode to be used
*iIncludins debouncing
*
*Pin connections included in submission
*/
#include <avr/io.h>
#include <util/delay.h>

#define BUTTON_PIN   PD2
#define BUTTON_PORT  PORTD
#define BUTTON_INPUT PIND
#define SEG_DDR      DDRB
#define SEG_PORT     PORTB

// Common anode digit table (0=segment on, 1=segment off)
// PB0=E, PB1=D, PB2=C, PB3=DP, PB4=B, PB5=A, PB6=F
const uint8_t digitTable[16] = {
    0b00100000, // 0
    0b11101011, // 1
    0b01001100, // 2
    0b01001001, // 3
    0b10001011, // 4
    0b00011001, // 5
    0b00011000, // 6
    0b01101011, // 7
    0b00001000, // 8
    0b00001001, // 9
    0b00001010, // A
    0b10011000, // b
    0b00111100, // C
    0b11001000, // d
    0b00011100, // E
    0b00011110, // F
};


int main(void) {
    SEG_DDR = 0xFF;       // PB0–PB6 outputs
    SEG_PORT = 0xFF;      // start with all segments off (common anode)

    BUTTON_PORT |= (1 << BUTTON_PIN);  // enable pull-up resistor

    uint8_t counter = 0;
    uint8_t lastButtonState = 1;

    while (1) {
        uint8_t currentButtonState = (BUTTON_INPUT & (1 << BUTTON_PIN)) >> BUTTON_PIN;

        // Detect button press (HIGH → LOW)
        if (lastButtonState == 1 && currentButtonState == 0) {
            _delay_ms(20);  // debounce
            if ((BUTTON_INPUT & (1 << BUTTON_PIN)) == 0) {
                counter++;
                if (counter > 15) counter = 0;      // wrap 0–15
                SEG_PORT = digitTable[counter];    // show digit

                // Wait for release
                while ((BUTTON_INPUT & (1 << BUTTON_PIN)) == 0) {
                    _delay_ms(10);
                }
            }
        }

        lastButtonState = currentButtonState;
    }
}
