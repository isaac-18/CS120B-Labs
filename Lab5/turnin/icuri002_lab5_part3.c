/*	
 * 	https://drive.google.com/drive/folders/1BRocQPJ9MgXXbSxM8Q3ydcosdjvfBOiH?usp=sharing
 *
 *	Author: icuri002
 *  	Partner(s) Name: 
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

enum States {Start, Init, Alt_Flash1, Alt_Flash2} state;
unsigned char A;
unsigned char B;
unsigned char i;

void tick() {
	switch(state) {
		case Start:
			state = Init;
			break;

		case Init:
			if (A == 0x01) {
				state = Alt_Flash1;
			}
			else {
				state = Init;
			}
			break;

		case Alt_Flash1:
			if (A == 0x01) {
				state = Alt_Flash1;
			}
			else if (A == 0x00) {
				state = Alt_Flash2;
			}
			else {
				state = Alt_Flash1;
			}
			break;

		case Alt_Flash2:
			if (A == 0x01) {
				state = Alt_Flash1;
			}
			else if (A == 0x00) {
				state = Alt_Flash2;
			}
			else {
				state = Alt_Flash2;
			}
			break;
		default:
			state = Start;
			break;
	}
	
	switch(state) {
		case Init:
			B = 0x00;	
			break;

		case Alt_Flash1:
			B = 0x15;
			break;

		case Alt_Flash2:
			B = 0x2A;
			break;
		default:
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;	// Configure as inputs
    DDRB = 0xFF; PORTB = 0x00;	// Configure as outputs; initialize C to 7	

    state = Start;
    B = 0x00; 
   
    /* Insert your solution below */
    while (1) {
	A = ~PINA & 0x01;
	tick();	
	PORTB = B;
    }
    return 1;
}
