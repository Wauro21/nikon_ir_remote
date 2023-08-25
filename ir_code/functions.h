#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/// @brief Defines the possibles states controlling the Timer0 pinout
typedef enum {
    IDLE    = 0U,
    ON      = 1U,
    OFF     = 2U,
} t0_pinout_states;


/// @brief Configures timer 0 to generate a 38kHz square wave.
/// @param  void
void setupTimer0(void);

#endif