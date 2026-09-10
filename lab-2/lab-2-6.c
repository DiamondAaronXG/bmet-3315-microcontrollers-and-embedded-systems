/*
*This code causes the 7 segment display to count from 0-9 then A-F at 2HZ
*
*Changed delay time defined in line 16 to "2" milliseconds
*(Not part of lab explicitely)
*
*Pin connections included in submission
*/
/* Increment counter on button press and display value on LEDs */

#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>

#define LED_DDR      DDRB
#define LED_PORT     PORTB

// Common anode digit table (0=segment on, 1=segment off)
// 
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
    int counter;
    LED_DDR = 0xFF;                     // Set all PORTB pins as outputs
    LED_PORT = 0x00;                    // Start with all LEDs off


while(1) {
    while (1) {
        LED_PORT = counter;
        counter++;
        if (counter > 15)
            counter = 0;
        LED_PORT = digitTable[counter];    // show digit
    _delay_ms(500);
    }
}
    return 0;
}
