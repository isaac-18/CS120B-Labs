/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #3  Exercise #2
 *	Exercise Description: A car has a fuel-level sensor that sets PA3..PA0 to a value between
 *	0 (empty) and 15 (full). A series of LEDs connected to PC5..PC0 should light to 
 *	graphically indicate the fuel level.
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
    DDRC = 0xFF; PORTC = 0x00;	// Configure port C's 8 pins as outputs; initialize to 0s

    unsigned char tmpA = 0x00;
    unsigned char tmpC = 0x00;
    unsigned char lowFuel = 0x00;

    /* Insert your solution below */
    while (1) {
	tmpA = ~PINA & 0x0F;
	tmpC = 0x00;

	if (tmpA <= 4) {	// Low fuel
		tmpC = 0x40; 

		if ((tmpA == 1) || (tmpA == 2)) {
			tmpC = tmpC | 0x20;	
		}
		else if ((tmpA == 3) || (tmpA == 4)) {
			tmpC = tmpC | 0x30;
		}
	}
	else {
		if ((tmpA == 5) || (tmpA == 6)) {
			tmpC = 0x38;
		}
		
		else if ((tmpA >= 7) && (tmpA <= 9)) {
			tmpC = 0x3C;
		}
		else if ((tmpA >= 10) && (tmpA <= 12)) {
			tmpC = 0x3E;
		} 	
		else if ((tmpA >=13) && (tmpA <= 15)) {
			tmpC = 0x3F;
		}
	} 
	PORTC = tmpC;
    }
    return 1;
}
