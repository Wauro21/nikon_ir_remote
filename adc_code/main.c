#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t pin_enable = 0U;
volatile uint16_t t1_interrupt_counter = 0U; /// 16 bits for larger times
volatile uint16_t t1_max_counter_value = 0U;
volatile uint8_t state = 0U;
volatile uint8_t low_adc_output = 0U;
volatile uint16_t adc_output = 0U;

int main(void)
{
    uint8_t t1_enable = 0U; // Disabled by default
    uint16_t t1_evaluation_value = 0U;


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
    // ADMUX |= _BV(ADLAR);
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


    /// Configure Timer 1
    TCCR1 = 0;
    GTCCR = 0;
    TCNT1 = 0;
    TIMSK = 0;

    /// Enable ctc on timer 1
    // TCCR1 |= _BV(CTC1);
    
    // TCCR1 |= _BV(CS13) | _BV(CS12) | _BV(CS11) | _BV(CS10); /// prescaler 16384

    TIMSK |= _BV(TOIE1);



    /// Start first conversion
    ADCSRA |= _BV(ADSC);

    sei();

    while(1){
        switch(state){
            case 0U: 
                /// Turns off the counter part
                t1_enable = 0U;
                t1_max_counter_value = 0U;
                t1_evaluation_value = 255U; /// imposible value while stopped
                break;

            case 1U:
                t1_enable = 1U;
                t1_max_counter_value = 1U;
                t1_evaluation_value = 0U;
                break;

            case 2U:
                t1_enable = 1U;
                t1_max_counter_value = 2U;
                t1_evaluation_value = 0U;
                break;

            case 3U:
                t1_enable = 1U;
                t1_max_counter_value = 4U;
                t1_evaluation_value = 0U;
                break;
        }

        /// Timer manipulation
        if(t1_enable){
            /// Update timer status
            TCCR1 |= _BV(CS13) | _BV(CS12) | _BV(CS11) | _BV(CS10);
            if(t1_interrupt_counter == t1_evaluation_value) PORTB |= _BV(PB0);
            else PORTB &= ~_BV(PB0);
        }

        else{
            /// Disable timer 
            TCCR1 = 0;
            PORTB &= ~_BV(PB0);
        }

        // if(t1_interrupt_counter == 0U) PORTB |= _BV(PB0);
        // else PORTB &= ~_BV(PB0);
    };
    return 0;
}

ISR(ADC_vect){

    low_adc_output = ADCL;
    adc_output = ADCH << 8 | low_adc_output;

    switch (adc_output)
    {
    case 0U ... 255U:
        state = 0U;
        break;

    case 256U ... 511U:
        state = 1U;
        break;
    
    case 512U ... 767U:
        state = 2U;
        break;

    case 768U ... 1023U:
        state = 3U;
        break;

    }
    // if(ADCH <= 10U) pin_enable = 1U;
    // else pin_enable = 0U;
}

ISR(TIMER1_OVF_vect){
    if(t1_interrupt_counter < t1_max_counter_value) t1_interrupt_counter += 1;
    else t1_interrupt_counter = 0;
}