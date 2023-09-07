#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <avr/io.h>

/// Parameters
#define TIME_SELECTOR_IO ADC3D

/// -> I/O parameters
/// ---> Inputs
#define MAIN_BUTTON PINB2
#define POTENTIOMETER PB3
/// ---> Outputs
#define IR_LED PB0
#define STATUS_LED PB4
#define POT_VCC PB1

/// ->Debouncer parameters
#define PRESS_BUTTON_MASK 0x80000000
#define HOLD_BUTTON_MASK 0xFFFFFFFF


/// Variables
/// @brief Counts the number of match interrupts generated by Timer0
volatile uint16_t t0_interrupt_counter;
/// @brief Controls the current state of the modulation of the 38kHZ signal
volatile uint8_t t0_modulation_state;
/// @brief holds the number of times the ISR has been used for a single CMD
volatile uint8_t t0_cmd_counter;
/// @brief The number of Timer 1 interruptions by overflow
volatile uint16_t t1_interrupt_counter;
/// @brief The maximum number of interrupts for Timer 1
volatile uint16_t t1_max_counter_value;
/// @brief Holds the current-debounced state of the button
uint8_t button_state;
/// @brief Register for button debouncing
uint32_t button_debounce;


volatile uint8_t adc_status; /// Holds the adc status
volatile uint8_t t0_status;
/// @brief Holds the required state for the Timer0 out pin.
volatile uint8_t t0_out_enable;

//// STATES

/// @brief Defines the possible states of the debouncer
typedef enum {
    BUTTON_IDLE    =   0x00, /// No change on the button
    BUTTON_PRESS   =   0x01, /// A single pressed or a release after a hold
    BUTTON_HOLD    =   0x02, /// The button is being hold
} button_states;


/// Functions

/// @brief Debounces the value read from MAIN_BUTTON
/// @param  state The address of the button state variable.
/// @return uint8_t -> 1 if the button was pressed.
button_states readButton(uint32_t* state);


/// @brief Sets the ADCSRA register to enable the ADC
/// @param  void
void enableADC(void);

/// @brief Stops the ADC by seting the ADCSRA register
/// @param  void
void stopADC(void);

/// @brief Trigger an ADC read by setting the ADCSRA register
/// @param  void
void triggerADC(void);

/// @brief Enables the ADC and starts a conversion to read the value of selector input
/// @param  void
void readInterval(void);

/// @brief Starts the timer 1 to generate an overflow interrupt with a frequency 1.9 Hz
/// @param  void
void startTimer1(void);

/// @brief Stops timer 1
/// @param  void
void stopTimer1(void);

/// @brief Configures timer 0 to generate a 38kHz square wave.
/// @param  void
// void setupTimer0(void);

void sendCMD(void);


void setupIO(void);


void setupADC(void);

void setupTimer0(void);

void setupTimer1(void);

void setupTimerInterrupts(void);

void enablePot(void);

void disablePot(void);


void powerReduction(void);

void configureWakeUp(void);

void enableInt0Interrupt(void);

void disableInt0Interrupt(void);

#endif