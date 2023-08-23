#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t isr_counter = 0;

int main(void)
{
    DDRB |= _BV(PB0) | _BV(PB1) | _BV(PB2);
    PORTB &= ~_BV(PB1);
    // Set general register to sync mode for configuration
    GTCCR |= _BV(TSM) | _BV(PSR0);
    // Default registers
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    TIMSK = 0;
    TCCR0A |= _BV(COM0A0) | _BV(WGM01); // Set toggle on match and CTC mode
    TCCR0B |= _BV(CS00);                // internal clock no-prescaling
    OCR0A = 103;                        // For app 38,8kHz
    TIMSK |= _BV(OCIE0A);
    // Enable timer
    GTCCR &= ~_BV(TSM) & ~_BV(PSR0);
    // Enable global interrupts
    sei();
    while (1);
}
// Demo interrupt service routine
ISR(TIMER0_COMPA_vect)
{
    if(isr_counter < 19U) isr_counter += 1;
    else{
        TCCR0A ^= _BV(COM0A0);
        isr_counter = 0;
    }
    // PORTB ^= _BV(PB1);
}
