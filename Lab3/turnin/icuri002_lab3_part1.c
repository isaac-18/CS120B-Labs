/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #3  Exercise #1
 *	Exercise Description: Count the number of 1s on ports A and B and output that number on port C. 
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
    DDRB = 0x00; PORTB = 0xFF;	// Configure port B's 8 pins as inputs
    DDRC = 0xFF; PORTC = 0x00;	// Configure port C's 8 pins as outputs; initialize to 0s

    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char tmpC = 0x00;

    unsigned char i;

    /* Insert your solution below */
    while (1) {
	tmpA = PINA & 0xFF;
	tmpB = PINB & 0xFF;
	tmpC = 0;
	for (i = 0; i < 8; ++i) {
		tmpC += tmpA & 0x01;
		tmpC += tmpB & 0x01;
		tmpA = tmpA >> 1;
		tmpB = tmpB >> 1;
	}
	PORTC = tmpC;
    }
    return 1;
}
