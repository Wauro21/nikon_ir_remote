#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t isr_counter = 0U;
volatile uint8_t state = 0U;
volatile uint8_t enable = 0U;

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
    OCR0A = 104;                        // For app 38,8kHz
    TIMSK |= _BV(OCIE0A);
    // Enable timer
    GTCCR &= ~_BV(TSM) & ~_BV(PSR0);
    // Enable global interrupts
    sei();
    while (1){
        if(enable == 1) {
            TCCR0A |= _BV(COM0A0);
            enable = 0;
        }

        else if(enable == 2) {
            TCCR0A &= ~_BV(COM0A0);
            enable = 0;
        }
    };
}
// Demo interrupt service routine
ISR(TIMER0_COMPA_vect)
{
    switch (state)
    {
        case 0:
            if(isr_counter < 151U) isr_counter += 1;
            else {
                enable = 2U;
                isr_counter = 0;
                state = 1;
            }
            break;

        case 1:
            if(isr_counter < 2127U) isr_counter += 1;
            else {
                enable = 1U;
                isr_counter = 0;
                state = 2;
            }
            break;

        case 2:
            if(isr_counter < 29U) isr_counter += 1;
            else {
                enable = 2U;
                isr_counter = 0;
                state = 3;
            }
            break;

        case 3:
            if(isr_counter < 119U) isr_counter += 1;
            else {
                enable = 1U;
                isr_counter = 0;
                state = 4;
            }
            break;
        
        case 4:
            if(isr_counter < 29U) isr_counter += 1;
            else {
                enable = 2U;
                isr_counter = 0;
                state = 5;
            }
            break;

        case 5:
            if(isr_counter < 271U) isr_counter += 1;
            else {
                enable = 1U;
                isr_counter = 0;
                state = 6;
            }
            break;

        case 6:
            if(isr_counter < 29U) isr_counter += 1;
            else {
                enable = 2U;
                isr_counter = 0;
                state = 7;
            }
            break;

        case 7:
            if(isr_counter < 4853U) isr_counter += 1;
            else {
                enable = 1U;
                isr_counter = 0;
                state = 0;
            }
            break;
    }
}
