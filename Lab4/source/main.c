/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #4  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Locked, Hash_Pressed, Wait1, Wait2, Getting_Input, Hash_Released, Unlocked} state;
unsigned char A;
unsigned char B;
unsigned char C;
unsigned char isUnlocked;

void tick() {
	switch(state) {
		case Start:
			state = Locked;
			break;

		case Locked:
			if ((A & 0x04) == 0x04) {
				state = Hash_Pressed;
			}
			else {
				state = Locked;
			}
			break;

		case Hash_Pressed:
	/*		if (A == 0x00) {
				state = Hash_Released;
			}
			else {
				state = Locked;
			}
	*/	
			state = Wait1;
			break;

		case Wait1:
			if (A == 0x00) {
				state = Hash_Released;
			}
			else {
				state = Locked;
			}
			break;
	
		case Hash_Released:
	/*		if ((A & 0x02) == 0x02) {
				state = Y_Pressed;
			}
			else {
				state = Locked;
			}
	*/	
			state = Wait2;
			break;

		case Wait2:	
			if ((A & 0x02) == 0x02) {
				state = Unlocked;
			}
			else {
				state = Locked;
			}
			break;
	
		case Unlocked:
			if (((A & 0x80) == 0x80) || isUnlocked) {
				state = Locked;
			}
			else {
				state = Unlocked;
			}
			break;


		default:
			state = Start;
			break;
	}
	
	switch(state) {
		case Locked:
			B = 0x00;
			C = 0x01;	
			break;

		case Hash_Pressed:
			C = 0x02;
			break;

		case Hash_Released:
			C = 0x03;
			break;

		case Unlocked:
			B = 0x01;
			C = 0x04;
			isUnlocked = !(isUnlocked);
			break;

		default:
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;	// Configure as inputs
    DDRB = 0xFF; PORTB = 0x00;	// Configure as outputs
    DDRC = 0xFF; PORTC = 0x00;	// Configure as outputs; initialize C to 7	

    state = Start;
    B = 0x00;	// Assumed to be initially locked
    C = 0x01; 	// Starting state is Locked i.e. 1
    isUnlocked = 0x01;
   
    /* Insert your solution below */
    while (1) {
	A = PINA & 0x87;
	tick();	
	PORTB = B;
	PORTC = C;
    }
    return 1;
}
