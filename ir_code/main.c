#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t isr_counter = 0U;
volatile uint8_t state = 0U;
volatile uint8_t round_count = 0;

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
    switch (state)
    {
    case 0U:
        if (isr_counter < 151U)
            isr_counter += 1U;
        else
        {
            TCCR0A &= ~_BV(COM0A0);
            isr_counter = 0U;
            state = 1U;
        }
        break;

    case 1U:
        if (isr_counter < 2127U)
            isr_counter += 1U;
        else
        {
            TCCR0A |= _BV(COM0A0);
            isr_counter = 0U;
            state = 2U;
        }
        break;

    case 2U:
        if (isr_counter < 29U)
            isr_counter += 1U;
        else
        {
            TCCR0A &= ~_BV(COM0A0);
            isr_counter = 0U;
            state = 3U;
        }
        break;

    case 3U:
        if (isr_counter < 119U)
            isr_counter += 1U;
        else
        {
            TCCR0A |= _BV(COM0A0);
            isr_counter = 0U;
            state = 4U;
        }
        break;

    case 4U:
        if (isr_counter < 29U)
            isr_counter += 1U;
        else
        {
            TCCR0A &= ~_BV(COM0A0);
            isr_counter = 0U;
            state = 5U;
        }
        break;

    case 5U:
        if (isr_counter < 271U)
            isr_counter += 1U;
        else
        {
            TCCR0A |= _BV(COM0A0);
            isr_counter = 0U;
            state = 6U;
        }
        break;

    case 6U:
        if (isr_counter < 29U)
            isr_counter += 1U;
        else
        {
            TCCR0A &= ~_BV(COM0A0);
            isr_counter = 0U;
            state = 7U;
        }
        break;

    case 7U:
        if (isr_counter < 4854U)
            isr_counter += 1U;
        else
        {
            if(round_count < 1U){
                TCCR0A |= _BV(COM0A0);
                state = 0U;
                round_count += 1U;
            }
            else {
                state = 8U;
                round_count = 0U;
            }
            isr_counter = 0U;
            
        }
        break;

    case 8U:
        if (isr_counter < 65000U)
            isr_counter += 1U;
        else
        {
            TCCR0A |= _BV(COM0A0);
            isr_counter = 0U;
            state = 0U;
        }
        break;

    }
    // if(isr_counter < 19) isr_counter += 1;
    // else{
    //     if(state == 0) {
    //         TCCR0A &= ~_BV(COM0A0);
    //         state = 1;
    //     }
    //     else
    //     {
    //         TCCR0A |= _BV(COM0A0);
    //         state = 0;
    //     }
    //     // TCCR0A ^= _BV(COM0A0);
    //     isr_counter = 0;
    // }
}
