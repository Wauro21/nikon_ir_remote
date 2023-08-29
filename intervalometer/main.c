#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

#define TIME_SELECTOR_IO ADC3D

volatile uint16_t t1_max_counter_value = 0U;
volatile uint8_t t1_enable = 0U;


int main(void){
    adc_status = 0x00;

    /// TEMPORAL -> Enable when pin is high
    DDRB = 0; /// Inputs -> PB3 potentiometer, PB4 button
    DDRB |= _BV(PB0); /// PB0 used for IR led.

    PORTB = 0; /// TEMPORAL
    PORTB |= _BV(PB4); // Enable pull up of PB4

    /// Interrupts 
    // MCUCR = 0;
    // MCUCR |= _BV(ISC01); /// Falling edge on pin generates interrupts

    GIMSK = 0;
    // GIMSK |= _BV(PCIE); /// Enable pin change interrupt

    PCMSK = 0;
    PCMSK |= _BV(PCINT4); /// Enable pin change interrupt on PB4


    /// ADC configurations
    ADMUX = 0;
    ADCSRA = 0;
    ADCSRB = 0;

    /// Set left adjust (8bit precision) and PB3 as input
    ADMUX |= _BV(ADLAR) | TIME_SELECTOR_IO;

    /// Enable ADC interrupts and select 128 prescaler for 62,5 kHz sampling freq
    ADCSRA |= _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    



    sei();

    while(1){
        if(readButton()){
            PORTB |= _BV(PB0);
            readInterval();
            PORTB &= ~_BV(PB0);
        }
    };

}

ISR(ADC_vect){
    /// Read the ADC value and set the mode
    switch(ADCH){
        case 0U ... 8U:
            t1_enable = 0U;
            t1_max_counter_value = 0U;
            break;
        
        case 9U ... 31:
            t1_enable = 1U;
            t1_max_counter_value = 1U; // 1sec

    }

    /// Set the status flag to escape read-loop
    adc_status = 0x01;
}