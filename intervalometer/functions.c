#include "functions.h"

/// Button control

uint8_t readButton(uint32_t* state){
    uint8_t btn_read = (~PINB & _BV(MAIN_BUTTON))>>MAIN_BUTTON;
    /// Read and shift for debouncing
    *state = (*state << 1) | btn_read;
    if(*state == 0x80000000) return 0x01;
    else if (*state == 0xFFFFFFFF) return 0x02;
    else return 0x00;

    // return (uint8_t)(state == 0x80000000);
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


void startTimer1(void){
    /// Starts the timer 1 with system clock and prescaler of 16324
    TCCR1 |= _BV(CS13) | _BV(CS12) | _BV(CS11) | _BV(CS10);
}


void stopTimer1(void){
    TCCR1 = 0;
}


/// @brief Setups the timer 0 register to generate a 38kHz square wave.
/// @brief acts as the carrier of the IR signal.
/// @param  void
// void setupTimer0(void)
// {
//     // Default registers
//     TCCR0A = 0;
//     TCCR0B = 0;
//     TCNT0 = 0;
//     TIMSK = 0;
//     TCCR0A |= _BV(COM0A0) | _BV(WGM01); // Set toggle on match and CTC mode
//     TCCR0B |= _BV(CS00);                // internal clock no-prescaling
//     OCR0A = 104;                        // For app 38,8kHz
//     return;
// }

void startTimer0(void){
    TCCR0B |= _BV(CS00);
}

void stopTimer0(void){
    TCCR0B &= ~_BV(CS00);
}


void sendCMD(void){
    /// Start timer 0
    t0_status = 0x01;
    startTimer0();
    /// Trap CPU and update pin accordingly
    while(t0_status == 0x01){
        if(t0_out_enable == 0x01) TCCR0A |= _BV(COM0A0);
        else TCCR0A &= ~_BV(COM0A0);
    }

    stopTimer0();
}