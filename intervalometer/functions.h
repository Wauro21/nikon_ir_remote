#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <avr/io.h>

/// Parameters
#define MAIN_BUTTON PINB4

/// Variables
volatile uint8_t adc_status; /// Holds the adc status

/// Functions

/// @brief Debounces the value read from MAIN_BUTTON
/// @param  void
/// @return uint8_t -> 1 if the button was pressed.
uint8_t readButton(void);


/// @brief Sets the ADCSRA register to enable the ADC
/// @param  void
void enableADC(void);

/// @brief Stops the ADC by seting the ADCSRA register
/// @param  void
void stopADC(void);

/// @brief Trigger an ADC read by setting the ADCSRA register
/// @param  void
void triggerADC(void);

void readInterval(void);

#endif