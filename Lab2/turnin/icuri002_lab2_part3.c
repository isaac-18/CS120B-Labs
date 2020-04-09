/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #2  Exercise #2
 *	Exercise Description: Port A's pins 3 to 0, each connect to a parking space sensor, 
 *	1 meaning a car is parked in the space, of a four-space parking lot. Write a 
 *	program that outputs in binary on port C the number of available spaces
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
    unsigned char tmpA = 0x00;
    unsigned char cntAvail;
    unsigned char i;
    unsigned char isFull;

    /* Insert your solution below */
    while (1) {
	tmpA = PINA & 0x0F;
	cntAvail = 0;
	isFull = 0;
	if (tmpA != 0x00) {
		for (i = 0; i < 4; ++i) {
			cntAvail += !(tmpA & 0x01);
			tmpA = tmpA >> 1;
		}
		if (cntAvail == 0) {
			isFull = 0x80;	
		} 
	} else {
		cntAvail = 4;
	}
	PORTC = cntAvail | isFull;
    }
    return 0;
}
