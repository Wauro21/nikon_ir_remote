#include "main.h"
#include <avr/io.h>
#include <util/delay.h>


int main (void){
    /* set pin 3 of PORTB for output*/
    DDRB |= _BV(DDB3);
    
    
    while(1) {
    /* set pin 3 high to turn led on */
    PORTB |= _BV(PORTB3);
    _delay_ms(BLINK_DELAY_MS);

    /* set pin 3 low to turn led off */
    PORTB &= ~_BV(PORTB3);
    _delay_ms(BLINK_DELAY_MS);
 }
}