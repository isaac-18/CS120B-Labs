/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, OFF_Release, ON_Press, ON_Release, OFF_Press} state;
unsigned char A0;
unsigned char tmpB;

void tick() {
	switch(state) {
		case Start:
			state = OFF_Release;
			break;

		case OFF_Release:
			if (A0) {
				state = ON_Press;
			}
			else if (!A0) {
				state = OFF_Release;
			}
			break;

		case ON_Press:
			if (!A0) {
				state = ON_Release;
			}
			else if (A0) {
				state = ON_Press;
			}	
			break;

		case ON_Release:
			if (A0) {
				state = OFF_Press;
			}
			else if (!A0) {
				state = ON_Release;
			}
			break;

		case OFF_Press:
			if (!A0) {
				state = OFF_Release;
			}
			else if (A0) {
				state = OFF_Press;
			}
			break;

		default:
			state = Start;
			break;
	}
	
	switch(state) {
		case OFF_Release:
			tmpB = 0x01;	
			break;

		case ON_Press:
			tmpB = 0x02;
			break;

		case ON_Release:
			break;

		case OFF_Press:
			tmpB = 0x01;

		default:
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;	// Configure as inputs
    DDRB = 0xFF; PORTB = 0x01;	// Configure as outputs; initialize B0 to 1	

//    unsigned char tmpA;
//    unsigned char tmpB;

//    B = 0x01;
    state = Start;
    
    /* Insert your solution below */
    while (1) {
	A0 = PINA & 0x01;
	tick();	
	PORTB = tmpB;
    }
    return 1;
}
