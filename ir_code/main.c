#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>

volatile uint8_t state = 0;

int main(void)
{

    // Configure output pins
    DDRB |= _BV(PB0); // Set pin as an output
    DDRB |= _BV(PB1);
    DDRB |= _BV(PB2);

    PORTB &= ~_BV(PB1);

    // Try using the timer0 to generate the 38.4 kHz carrier
    // [CONFIGURE TIMER]
    // -> GTCCR - General timer configs
    GTCCR &= ~_BV(TSM);
    GTCCR &= ~_BV(PSR0);

    // -> Timer registers
    TCCR0A = 0;
    TCCR0B = 0;
    TCCR0A |= _BV(WGM01);  // Set CTC mode of operation
    TCCR0A |= _BV(COM0A0); // Toggle on Compare match -> PB0 (Pin5)

    TCCR0B |= _BV(CS00); // internal clock with no prescaling

    TCNT0 = 0; // Start the counter at zero

    OCR0A = 12; // For 38,462kHz

    TIMSK |= _BV(OCIE0A);


    while(1){
        PORTB ^= _BV(PB2);
    }

}


ISR(TIMER0_COMPA_vect)
{
    PORTB ^= _BV(PB1);
    
}