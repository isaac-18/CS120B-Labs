/*	
 *	https://drive.google.com/file/d/1HYuFX2cst1_7RK-Kx1CCkuFBdAVVJvVS/view?usp=sharing
 *
 *	Author: icuri002

 *  	Partner(s) Name: 

 *	Lab Section: 024

 *	Assignment: Lab #6  Exercise #3

 *	Exercise Description: Increment and decrement w/ button press every 100ms

 *

 *	I acknowledge all content contained herein, excluding template or example

 *	code, is my own original work.

 */

#include <avr/io.h>

#include <avr/interrupt.h>

#ifdef _SIMULATE_

#include "simAVRHeader.h"

#endif



enum States {Start, Init, Increment, Decrement, Reset, Wait, Both_Released, A1_Held, A2_Held} state;

unsigned char A;

unsigned char tmpB;

unsigned char i;



volatile unsigned char TimerFlag = 0;	// TimerISR() sets this to 1. C programmer should clear to 0.



// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.

unsigned long _avr_timer_M = 1;	// Start count from here, down to 0. Default to 1 ms.

unsigned long _avr_timer_cntcurr = 0;	// Current internal count of 1ms ticks



void TimerOn() {

	TCCR1B = 0x0B;

	

	OCR1A = 125;

	

	TIMSK1 = 0x02;

	

	TCNT1 = 0;



	_avr_timer_cntcurr = _avr_timer_M;



	SREG |= 0x80;

}



void TimerOff() {

	TCCR1B = 0x00;

}



void TimerISR() {

	TimerFlag = 1;

}



ISR(TIMER1_COMPA_vect) {

	_avr_timer_cntcurr--;

	if (_avr_timer_cntcurr == 0) {

		TimerISR();

		_avr_timer_cntcurr = _avr_timer_M;

	}

}



void TimerSet(unsigned long M) {

	_avr_timer_M = M;

	_avr_timer_cntcurr = _avr_timer_M;

}





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

			if (A == 0x01) {

				i = 0;

				state = A1_Held;

			}

			else {

				state = Wait;

			}

			break;



		case Decrement:

			if (A == 0x02) {

				i = 0;

				state = A2_Held;

			}

			else {

				state = Wait;

			}

			break;



		case A1_Held:

			if (i <= 6) {

				state = A1_Held;

			}

			else {

				state = Increment;

			}

			break;



		case A2_Held:

			if (i <= 7) {

				state = A2_Held;

			}

			else {

				state = Decrement;

			}

			break;



		case Wait:

			if (A == 0x03) {

				state = Reset;

			}

			else if (A == 0x02) {

				state = Decrement;

			}

			else if (A == 0x01) {

				state = Increment;

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

			tmpB = 0x07;	

			break;



		case Increment:

			if (tmpB < 9) {

				tmpB = tmpB + 1;

			}

			break;



		case Decrement:

			if (tmpB > 0) {

				tmpB = tmpB - 1;

			}

			break;



		case A1_Held:

			++i;

			break;



		case A2_Held:

			++i;

			break;



		case Reset:

			tmpB = 0x00;

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

    tmpB = 0x07; 

    TimerSet(90);

    TimerOn();

   

    /* Insert your solution below */

    while (1) {

	A = ~PINA & 0x03;

	tick();	

	PORTB = tmpB;

	while(!TimerFlag);

	TimerFlag = 0;

    }

    return 1;

}
