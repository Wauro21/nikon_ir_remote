#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <avr/io.h>

/// Parameters
#define MAIN_BUTTON PINB2

/// Variables
volatile uint8_t adc_status; /// Holds the adc status
volatile uint8_t t0_status;
/// @brief Holds the required state for the Timer0 out pin.
volatile uint8_t t0_out_enable;

/// Functions

/// @brief Debounces the value read from MAIN_BUTTON
/// @param  void
/// @return uint8_t -> 1 if the button was pressed.
uint8_t readButton(uint32_t* state);


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

#endif