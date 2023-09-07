#include "functions.h"


button_states readButton(uint32_t* state){
    /// The button has negative logic, a press corresponds to a low level
    uint8_t btn_read = (~PINB & _BV(MAIN_BUTTON))>>MAIN_BUTTON;
    /// The read value in inverted for positive logic and shifted
    /// Read and shift for debouncing
    *state = (*state << 1) | btn_read;
    switch(*state){
        case PRESS_BUTTON_MASK: return BUTTON_PRESS;
        case HOLD_BUTTON_MASK: return BUTTON_HOLD;
        default: return BUTTON_IDLE;
    }
}


/// ADC

void enableADC(void){
    PRR &= ~_BV(PRADC);
    ADCSRA |= _BV(ADEN);
}

void stopADC(void){
    ADCSRA &= ~_BV(ADEN);
    PRR |= _BV(PRADC);
}

void triggerADC(void){
    ADCSRA |= _BV(ADSC);
}

void readInterval(void){
    /// enable and start the ADC
    enablePot();
    enableADC();
    triggerADC();

    /// Trap the CPU until the ADC is performed
    while(adc_status == 0x00);

    /// State value is set, clean adc_status flag
    adc_status = 0x00;

    // Stop adc
    stopADC();
    disablePot();
}


void startTimer1(void){
    /// Power timer 1
    PRR &= ~_BV(PRTIM1);

    /// Starts the timer 1 with system clock and prescaler of 16324
    TCCR1 |= _BV(CS13) | _BV(CS12) | _BV(CS11) | _BV(CS10);
}


void stopTimer1(void){
    TCCR1 = 0;

    /// Power off timer
    PRR |= _BV(PRTIM1);
}


/// @brief Setups the timer 0 register to generate a 38kHz square wave.
/// @brief acts as the carrier of the IR signal.
/// @param  void
void setupTimer0(void)
{
    // Default registers
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    TCCR0A |= _BV(COM0A0) | _BV(WGM01); // Set toggle on match and CTC mode
    OCR0A = 104;                        // For app 38,8kHz
    return;
}

void startTimer0(void){

    /// Power timer 0
    PRR &= ~_BV(PRTIM0);

    TCCR0B |= _BV(CS00);
}

void stopTimer0(void){
    TCCR0B &= ~_BV(CS00);

    /// Power off timer 0
    PRR |= _BV(PRTIM0);
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


void setupIO(void){
    DDRB = 0; /// Clear the reg
    /// Inputs
    DDRB &= ~_BV(MAIN_BUTTON) & ~_BV(POTENTIOMETER);

    /// Outputs
    DDRB |= _BV(IR_LED) | _BV(STATUS_LED) | _BV(POT_VCC);

    /// Clear output values enable pull-up for main-button
    PORTB = 0;
    PORTB |= _BV(PB2);
}

void enablePot(void){
    PORTB |= _BV(POT_VCC); /// powers the potentiometer
}

void disablePot(void){
    PORTB &= ~_BV(POT_VCC);
}


void setupADC(void){
    /// Clear register before configuration
    ADMUX = 0;
    ADCSRA = 0;
    ADCSRB = 0;

    /// Set left adjust (8bit precision) and PB3 as input
    ADMUX |= _BV(ADLAR) | TIME_SELECTOR_IO;

    /// Enable ADC interrupts and select 128 prescaler for 62,5 kHz sampling freq
    ADCSRA |= _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}



void setupTimer1(void){
    TCCR1 = 0;
    GTCCR = 0;
    TCNT1 = 0;
    TIMSK = 0;
}


void setupTimerInterrupts(void){
    TIMSK = 0;
    TIMSK |= _BV(TOIE1) | _BV(OCIE0A);
}


void powerReduction(void){
    PRR = 0;
    /// power everything off
    PRR |= _BV(PRTIM1) | _BV(PRTIM0) | _BV(PRUSI) | _BV(PRADC);
}

void configureWakeUp(void){
    /// Configure INT0 as interrupt
    MCUCR = 0;
    GIMSK = 0;
    GIMSK |= _BV(INT0); /// Enable int0 interrupt
}

void enableInt0Interrupt(void){
    GIMSK |= _BV(INT0);
}

void disableInt0Interrupt(void){
    GIMSK &= ~_BV(INT0);
}