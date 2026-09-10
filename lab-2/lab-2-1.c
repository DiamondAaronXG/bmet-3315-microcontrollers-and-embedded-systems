/*
*This code causes the LED bar graph to display a short "random" pattern
*
*Changed delay time defined in line 16 to "2" milliseconds
*(Not part of lab explicitely)
*
*PB0-PB7 connected to pins 1-8 on the LED bar
*/
// POV toy demo framework //

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>

// -------- Functions --------- //
void POVDisplay(uint8_t oneByte) {
  PORTB = oneByte;
  _delay_ms(2);
}

int main(void) {
  // -------- Inits --------- //
  DDRB = 0xff;                    /* Set up all of LED pins for output */
  // ------ Event loop ------ //
  while (1) {                                              /* mainloop */
    POVDisplay(0b00001110); // Turns all LEDs to on
    POVDisplay(0b00011000);
    POVDisplay(0b10111101);
    POVDisplay(0b01110110);
    POVDisplay(0b00111100);
    POVDisplay(0b00111100);
    POVDisplay(0b00111100);
    POVDisplay(0b01110110);
    POVDisplay(0b10111101);
    POVDisplay(0b00011000);
    POVDisplay(0b00001110);

    PORTB = 0; // Turns all PORTB off (LEDs)
    _delay_ms(10);
  }                                                    /* end mainloop */
  return 0;
}
