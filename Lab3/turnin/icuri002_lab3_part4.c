/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #3  Exercise #4
 *	Exercise Description: challenege exercise
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00;	// Configure port B's 8 pins as outputs; intialize to 0s
    DDRC = 0xFF; PORTC = 0x00;	// Configure port C's 8 pins as outputs; initialize to 0s

    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char tmpC = 0x00;

    while (1) {
	tmpA = PINA & 0xFF;
	tmpB = 0x00;
	tmpC = 0x00;

	tmpB = (tmpB & 0xF0) | (tmpA >> 4);
	tmpC = (tmpC & 0x0F) | (tmpA << 4);

	PORTB = tmpB;
	PORTC = tmpC;
    }
    return 1;
}
