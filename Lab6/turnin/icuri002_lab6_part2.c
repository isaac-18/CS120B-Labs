/* 
 *  https://drive.google.com/file/d/1GJKyWrMCFKZxG5x2EEoiXUZxijFZCFFd/view?usp=sharing 
 *
 *  Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: Blink three leds (B0, B1, B2) in sequence and user presses button (A0) to stop LED
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Blink1, Blink2, Button_Pressed, Button_Released} state;

unsigned char A;
unsigned char tmpB;

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

void Tick() {
	switch(state) {
		case Start:
			tmpB = 0x01;
			state = Blink1;
			break;

		case Blink1:
			if (A == 0x01) {
				tmpB = tmpB;
				state = Button_Pressed;
			}
			else if (!(tmpB >= 0x04)) {
				state = Blink1;
			}
			else if (tmpB >= 0x04) {
				state = Blink2;
			}
			break;

		case Blink2:
			if (A == 0x01) {
				tmpB = tmpB;
				state = Button_Pressed;
			}
			else if (!(tmpB <= 0x01)) {
				state = Blink2;
			}
			else if (tmpB <= 0x01) {
				state = Blink1;
			}
			break;

		case Button_Pressed:
			if (A != 0x01) {
				state = Button_Released;
			}
			else {
				state = Button_Pressed;
			}
			break;

		case Button_Released:
			if (A) {
				state = Blink1;
			}
			else {
				state = Button_Released;
			}
			break;

		default:
			state = Start;
			break;
	}

	switch(state) {
		case Start:
			break;

		case Blink1:
			tmpB = tmpB << 1;
			break;

		case Blink2:
			tmpB = tmpB >> 1;
			break;

		case Button_Pressed:
			break;

		case Button_Released:
			break;

		default:
			break;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	state = Start;
	tmpB = 0x01;
	TimerSet(300);
	TimerOn();

    /* Insert your solution below */
    while (1) {
	A = ~PINA & 0x01;
	Tick();
	PORTB = tmpB;
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
