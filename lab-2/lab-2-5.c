/*
*This code is a binary counter incremented by a NO button.
*The LED bar graph displays the binary equivalent vlaue carried by the variable
*"LED_PORT" defined in line 19
*
*Changed the entire code to allow for a push button to be used
*including the debouncing code
*
*PB0-PB7 connected to pins 1-8 on the LED bar
*/
/* Increment counter on button press and display value on LEDs */

#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>

#define BUTTON_PIN   PD2
#define BUTTON_PORT  PORTD
#define BUTTON_INPUT PIND
#define LED_DDR      DDRB
#define LED_PORT     PORTB

int main(void) {
    BUTTON_PORT |= (1 << BUTTON_PIN);   // Enable pull-up resistor on PD2
    LED_DDR = 0xFF;                     // Set all PORTB pins as outputs
    LED_PORT = 0x00;                    // Start with all LEDs off

    uint8_t counter = 0;
    uint8_t lastButtonState = 1;        // 1 = not pressed (pull-up keeps pin high)

    while (1) {
        uint8_t currentButtonState = (BUTTON_INPUT & (1 << BUTTON_PIN)) >> BUTTON_PIN;

        // Detects HIGH to LOW transitions
        if (lastButtonState == 1 && currentButtonState == 0) {
            _delay_ms(20);              // debounce wait
            // Checks again to confirm a button press
            if ((BUTTON_INPUT & (1 << BUTTON_PIN)) == 0) {
                counter++;              // Increment counter
                LED_PORT = counter;     // Show counter on LEDs
                // Waits until button is released
                while ((BUTTON_INPUT & (1 << BUTTON_PIN)) == 0) {
                    _delay_ms(10);
                }
            }
        }

        lastButtonState = currentButtonState;
    }

    return 0;
}
