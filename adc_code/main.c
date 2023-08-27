#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t pin_enable = 0U;

int main(void)
{
    /// PB3 is the read input and PB0 as output
    DDRB = 0;
    DDRB |= _BV(PB0);

    /// Default value of PB0
    PORTB = 0;

    /// Clean the ADC register before initialization
    ADMUX = 0; /// Sets vref to Vcc for single ended convertion
    ADCSRA = 0;
    ADCSRB = 0; /// this sets free running mode

    /// enable left adjust for 8 bit precision
    ADMUX |= _BV(ADLAR);
    /// Select PB3 as input 
    ADMUX |= ADC3D;

    /// Enable auto-trigger for testing
    ADCSRA |= _BV(ADATE);
    /// Enable ADC interrupt
    ADCSRA |= _BV(ADIE);
    /// select 1/128 prescaler -> 8 MHz/128 = 62,5 kHZ
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);


    /// Enable adc
    ADCSRA |= _BV(ADEN);

    /// Start first conversion
    ADCSRA |= _BV(ADSC);

    sei();

    while(1){
        if(pin_enable) PORTB |= _BV(PB0);
        else PORTB &= ~_BV(PB0);
    };
    return 0;
}

ISR(ADC_vect){
    if(ADCH > 127U) pin_enable = 1U;
    else pin_enable = 0U;
}