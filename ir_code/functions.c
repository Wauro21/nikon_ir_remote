#include "functions.h"
#include <avr/io.h>


/// @brief Setups the timer 0 register to generate a 38kHz square wave.
/// @brief acts as the carrier of the IR signal.
/// @param  void
void setupTimer0(void)
{
    // Default registers
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    TIMSK = 0;
    TCCR0A |= _BV(COM0A0) | _BV(WGM01); // Set toggle on match and CTC mode
    TCCR0B |= _BV(CS00);                // internal clock no-prescaling
    OCR0A = 104;                        // For app 38,8kHz
    return;
}