/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #2  Exercise #4
 *	Exercise Description: Amusment park ride challenge exercise.
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
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0x00; PORTC = 0xFF;
    DDRD = 0xFF; PORTD = 0x00;
    // DDRC = 0xFF; PORTC = 0x00;	// Configure port C's 8 pins as outputs, initialize to 0s
    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char tmpC = 0x00;
    unsigned char tmpD = 0x00;
    unsigned short sum = 0x0000;

    /* Insert your solution below */
    while (1) {
	tmpA = PINA & 0xFF;
	tmpB = PINB & 0xFF;
	tmpC = PINC & 0xFF;
	tmpD = 0x00;
	
	sum = tmpA + tmpB + tmpC;

	if (sum > 140) {
		tmpD = tmpD | 0x01;
	}
	if (((tmpA - tmpC) > 80) || ((tmpA - tmpC) < -80)) {
		tmpD = tmpD | 0x02;	
	}
	PORTD = (sum & 0x00FC) | (tmpD & 0x03);
    }
    return 0;
}
