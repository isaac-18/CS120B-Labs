/*
 *  https://drive.google.com/file/d/1COC8qAB5HjX04qoV8QzNqXOddQ7JU5-V/view?usp=sharing
 *
 *  Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Init, Increment, Decrement, Reset, Wait, Both_Released} state;
unsigned char A;
unsigned char tmpC;

void tick() {
	switch(state) {
		case Start:
			state = Init;
			break;

		case Init:
			if (A == 0x01) {
				state = Increment;
			}
			else if (A == 0x02) {
				state = Decrement;
			}
			else if (A == 0x03) {
				state = Reset;
			}
			else if (A == 0x00) {
				state = Init;
			}
			break;

	// Left off: Where to initialize C to 0x07? Also is it C or tmpC?
		case Increment:
			state = Wait;
			break;

		case Decrement:
			state = Wait;
			break;

		case Wait:
			if (A == 0x03) {
				state = Reset;
			}
			else if (A == 0x00) {
				state = Both_Released;
			}
			else {
				state = Wait;
			}
			break;

		case Both_Released:
			if (A == 0x03) {
				state = Reset;
			}
			else if (A == 0x01) {
				state = Increment;
			}
			else if (A == 0x02) {
				state = Decrement;
			}
			else if (A == 0x00) {
				state = Both_Released;
			}
			break;

		case Reset:
			if (A == 0x01) {
				state = Increment;
			}
			else if (A == 0x02) {
				state = Decrement;
			}
			else if (A == 0x03) {
				state = Reset;
			}
			else if (A == 0x00) {
				state = Both_Released;
			}
			break;

		default:
			state = Start;
			break;
	}
	
	switch(state) {
		case Init:
			tmpC = 0x00;	
			break;

		case Increment:
			if (tmpC < 9) {
				tmpC = tmpC + 1;
			}
			break;

		case Decrement:
			if (tmpC > 0) {
				tmpC = tmpC - 1;
			}
			break;

		case Reset:
			tmpC = 0x00;
			break;

		default:
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;	// Configure as inputs
    DDRC = 0xFF; PORTC = 0x00;	// Configure as outputs; initialize C to 7	

    state = Start;
    tmpC = 0x00; 
   
    /* Insert your solution below */
    while (1) {
	A = ~PINA & 0x03;
	tick();	
	PORTC = tmpC;
    }
    return 1;
}
