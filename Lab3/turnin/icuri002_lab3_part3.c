/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #3  Exercise #3
 *	Exercise Description: same as previous but includes seatbelt light
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

//    unsigned char tmpA = 0x00;
    unsigned char tmpAGas = 0x00;
    unsigned char tmpASeatBelt = 0x00;
    unsigned char tmpC = 0x00;

    /* Insert your solution below */
    while (1) {
	//tmpA = PINA & 0x7F;
	tmpAGas = PINA & 0x0F;
	tmpASeatBelt = PINA & 0x70;
	tmpC = 0x00;

	if (tmpAGas <= 4) {	// Low fuel
		tmpC = 0x40; 

		if ((tmpAGas == 1) || (tmpAGas == 2)) {
			tmpC = tmpC | 0x20;	
		}
		else if ((tmpAGas == 3) || (tmpAGas == 4)) {
			tmpC = tmpC | 0x30;
		}
	}
	else {
		if ((tmpAGas == 5) || (tmpAGas == 6)) {
			tmpC = 0x38;
		}
		
		else if ((tmpAGas >= 7) && (tmpAGas <= 9)) {
			tmpC = 0x3C;
		}
		else if ((tmpAGas >= 10) && (tmpAGas <= 12)) {
			tmpC = 0x3E;
		} 	
		else if ((tmpAGas >=13) && (tmpAGas <= 15)) {
			tmpC = 0x3F;
		}
	}
	if ((tmpASeatBelt & 0x70) == 0x30) {	// Fasten seatbelt
		tmpC = tmpC | 0x80;
	} 
	PORTC = tmpC;
    }
    return 1;
}
