#include "functions.h"

/// Button control

uint8_t readButton(void){
    static uint32_t state = 0;
    uint8_t btn_read = PINB & _BV(MAIN_BUTTON);
    
    /// Read and shift for debouncing
    state = (state << 1) | btn_read;
    return (uint8_t)(state == 0x80000000);
}


/// ADC

void enableADC(void){
    ADCSRA |= _BV(ADEN);
}

void stopADC(void){
    ADCSRA &= ~_BV(ADEN);
}

void triggerADC(void){
    ADCSRA |= _BV(ADSC);
}

void readInterval(void){
    /// enable and start the ADC
    enableADC();
    triggerADC();

    /// Trap the CPU until the ADC is performed
    while(adc_status == 0x00);

    /// State value is set, clean adc_status flag
    adc_status = 0x00;
}