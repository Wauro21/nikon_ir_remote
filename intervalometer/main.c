#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "functions.h"


/// TEMPORAL
volatile uint8_t stop_button_timer = 0U;
volatile uint8_t stop_button_flag = 0U;
uint8_t request_sleep = 0U;

int main(void){
    /// Initialize vars
    button_debounce = 0U;
    button_state = 0x00;
    t0_interrupt_counter = 0U;
    t0_modulation_state = 0U;
    t0_cmd_counter = 0U;
    t1_interrupt_counter = 0U;
    t1_max_counter_value = 0U;
    stop_button_timer = 0U;
    adc_status = 0x00;
    t0_status = 0x00;
    t0_out_enable = 0x00;


    /// WIP
    uint8_t adc_enable = 0x00;
    uint8_t busy = 0x00;
    uint8_t stop = 0x00;
    /// Configure registers
    setupIO();
    setupADC();
    setupTimer0();
    setupTimer1();
    setupTimerInterrupts();
    powerReduction();
    configureWakeUp();

    sei();


    /// -> Enter power down 
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
    

    while(1){

        /// sleep configuration
        if(request_sleep){
            request_sleep = 0U;
            enableInt0Interrupt();
            sleep_mode();
        }

        /// Check the button state
        button_state = readButton(&button_debounce);

        switch(button_state){
            case BUTTON_PRESS:
                if(busy == 0x00){
                    busy = 0x01;
                    adc_enable = 0x01;
                }

                if(stop == 0x01){
                    busy = 0x00;
                    stopTimer1();
                    t1_interrupt_counter = 0U;
                    stop_button_timer = 0U;
                    stop_button_flag = 0U;
                    PORTB &= ~_BV(PB4);
                    stop = 0x00;
                    request_sleep = 0x01;
                }
                break;

            case BUTTON_HOLD:
                if(busy == 0x01){
                    stop_button_flag = 0x01;
                    if(stop_button_timer == 9U){
                        PORTB |= _BV(PB4);
                        stop = 0x01;
                    }
                }
                break;

            default: break;
        }

        /// Start shot routine
        
        /// Reads the pot value
        if(adc_enable == 0x01){
            readInterval();
            adc_enable = 0x00;
        }

        /// Starts shoting
        if(busy){
            if(t1_max_counter_value == 0U){
                sendCMD();
                busy = 0x00;
                /// Enter power down
                request_sleep = 0x01;
            }
            else{
                startTimer1();
                if(t1_interrupt_counter == 0U) {
                    sendCMD();
                    /// Enter light sleep
                }
            }
        }
    };

}

ISR(INT0_vect){
    sleep_disable();
    disableInt0Interrupt();
}

ISR(ADC_vect){
    /// Read the ADC value and set the mode
    switch(ADCH){
        case 0U ... 8U:
            t1_max_counter_value = 0U;
            break;
        
        case 9U ... 21:
            t1_max_counter_value = 1U; // 1sec
            break;

        case 22 ... 34:
            t1_max_counter_value = 3U; // 2 sec
            break;

        case 35 ... 47:
            t1_max_counter_value = 5U; // 3 sec
            break;

        case 48 ... 60:
            t1_max_counter_value = 7U; // 4 sec
            break;

        case 61 ... 73:
            t1_max_counter_value = 9U; // 5 sec
            break;

        case 74 ... 86:
            t1_max_counter_value = 11U; // 6 sec
            break;

        case 87 ... 99:
            t1_max_counter_value = 13U; // 7 sec
            break;

        case 100 ... 112:
            t1_max_counter_value = 15U; // 8 sec
            break;

        case 113 ... 125:
            t1_max_counter_value = 17U; // 9 sec
            break;

        case 126 ... 138:
            t1_max_counter_value = 19U; // 10 sec
            break;

        case 139 ... 151:
            t1_max_counter_value = 39U; // 20 sec
            break;

        case 152 ... 164:
            t1_max_counter_value = 59U; // 30 sec
            break;

        case 165 ... 177:
            t1_max_counter_value = 79U; // 40 sec
            break;

        case 178 ... 190:
            t1_max_counter_value = 99U; // 50 sec
            break;

        case 191 ... 203:
            t1_max_counter_value = 119U; // 60 sec
            break;

        case 204 ... 216:
            t1_max_counter_value = 239U; // 120 sec
            break;

        case 217 ... 229:
            t1_max_counter_value = 359U; // 180 sec
            break;

        case 230 ... 242: 
            t1_max_counter_value = 479U; // 240 sec
            break;

        case 243 ... 255:
            t1_max_counter_value = 599U; // 300 sec
            break;

        default:
            t1_max_counter_value = 0U;
    };

    /// Set the status flag to escape read-loop
    adc_status = 0x01;
}


ISR(TIMER1_OVF_vect){
    if(t1_interrupt_counter < t1_max_counter_value) t1_interrupt_counter += 1;
    else {
        t1_interrupt_counter = 0;
        /// Exit power off
    }

    if(stop_button_flag) stop_button_timer += 1U;
}


/// @brief Generates the <shot> command by modulating the 38kHz wave generated by Timer0
/// @param void 
ISR(TIMER0_COMPA_vect){
    switch (t0_modulation_state)
    {
        case 0:
            if (t0_interrupt_counter < 151U) t0_interrupt_counter += 1;
            else{
                t0_out_enable = 0x00;
                t0_interrupt_counter = 0;
                t0_modulation_state = 1;
            }
            break;

        case 1:
            if (t0_interrupt_counter < 2127U) t0_interrupt_counter += 1;
            else{
                t0_out_enable = 0x01;
                t0_interrupt_counter = 0;
                t0_modulation_state = 2;
            }
            break;

        case 2:
            if (t0_interrupt_counter < 29U) t0_interrupt_counter += 1;
            else{
                t0_out_enable = 0x00;
                t0_interrupt_counter = 0;
                t0_modulation_state = 3;
            }
            break;

        case 3:
            if (t0_interrupt_counter < 119U) t0_interrupt_counter += 1;
            else{
                t0_out_enable = 0x01;
                t0_interrupt_counter = 0;
                t0_modulation_state = 4;
            }
            break;

        case 4:
            if (t0_interrupt_counter < 29U) t0_interrupt_counter += 1;
            else{
                t0_out_enable = 0x00;
                t0_interrupt_counter = 0;
                t0_modulation_state = 5;
            }
            break;

        case 5:
            if (t0_interrupt_counter < 271U) t0_interrupt_counter += 1;
            else{
                t0_out_enable = 0x01;
                t0_interrupt_counter = 0;
                t0_modulation_state = 6;
            }
            break;

        case 6:
            if (t0_interrupt_counter < 29U) t0_interrupt_counter += 1;
            else{
                t0_out_enable = 0x00;
                t0_interrupt_counter = 0;
                t0_modulation_state = 7;
            }
            break;

        case 7:
            if (t0_interrupt_counter < 4853U) t0_interrupt_counter += 1;
            else{
                if(t0_cmd_counter == 0U){
                    t0_out_enable = 0x01;
                    t0_interrupt_counter = 0;
                    t0_modulation_state = 0;
                    t0_cmd_counter += 1;
                }

                else{
                    t0_out_enable = 0x00;
                    t0_interrupt_counter = 0U;
                    t0_modulation_state = 0U;
                    t0_cmd_counter = 0U;
                    t0_status = 0x00;
                }
            }
            break;
    }
}
