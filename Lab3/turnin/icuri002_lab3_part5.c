/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #3  Exercise #5
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
    DDRD = 0x00; PORTD = 0xFF;	// Configure port A's 8 pins as inputs
    DDRB = 0xFE; PORTB = 0x01;	// Configure port B's 7 pins as outputs and LSB as input

    unsigned char tmpD = 0x00;
    unsigned char tmpB = 0x00;
    unsigned short weight = 0x00;

    while (1) {
	tmpD = PIND & 0xFF;
	tmpB = PINB & 0x01;
	
	weight = tmpD + tmpB;
	tmpB = 0;

	if (weight >= 70) {
		tmpB = 0x02;
	}
	else if ((weight > 5) && (weight < 70)) {
		tmpB = 0x04;
	}
	PORTB = tmpB;
    }
    return 1;
}
