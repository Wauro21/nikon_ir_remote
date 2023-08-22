#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)
{

    DDRB |= _BV(PB0) | _BV(PB1);
    PORTB &= ~_BV(PB1);

    // Set general register to sync mode for configuration
    // GTCCR |= _BV(TSM) | _BV(PSR0);

    // Default registers
    TCCR0A = 0; 
    TCCR0B = 0;
    TCNT0 = 0;

    TCCR0A |= _BV(COM0A0) | _BV(WGM01); // Set toggle on match and CTC mode
    TCCR0B |= _BV(CS00); // internal clock no-prescaling

    OCR0A |= 12; // For app 38,8kHz

    TIMSK |= _BV(OCIE0A);

    // Enable timer

    // GTCCR &= ~_BV(TSM) & ~_BV(PSR0);
    while(1);


}

ISR(TIMER0_COMPA_vect){
    PORTB ^= _BV(PB1);
}
