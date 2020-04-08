/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
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
    DDRC = 0xFF; PORTC = 0x00;	// Configure port C's 8 pins as outputs, initialize to 0s
    unsigned char tmpC = 0x00;
    unsigned char tmpA = 0x00;
    unsigned char cntAvail;
    unsigned char i;

    /* Insert your solution below */
    while (1) {
	tmpA = PINA & 0x0F;
	if (tmpA != 0x00) {
		for (i = 0; i < 4; ++i) {
			cntAvail += !(tmpA & 0x01);
			tmpA = tmpA >> 1;
		} 
	} else {
		cntAvail = 4;
	}
	PORTC = cntAvail;
    }
    return 0;
}
