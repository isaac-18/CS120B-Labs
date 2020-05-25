/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section:024
 *	Assignment: Lab #10  Exercise #3
 *	Exercise Description: shared SMs that blink one LED in one sec interval and three
 *	others in 300 ms interval. Now includes speaker that sounds on and off every 2ms 
 *	while switch is on.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;
unsigned char A;
unsigned char B;
unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char speaker;

unsigned char TL_cnt;
unsigned char BL_cnt;

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

enum TL_States {TL_Start, TL_Blink, TL_Wait} TL_State;
void Tick_ThreeLEDs () {
	switch(TL_State) {
		case TL_Start:
			threeLEDs = 0x01;
			TL_State = TL_Blink;
			break;	

		case TL_Blink:
			if (threeLEDs >= 4) {
				threeLEDs = 0x01;
			}
			else {
				threeLEDs = threeLEDs << 1;
			}
			TL_State = TL_Blink;
			break;
		
		default:
			TL_State = TL_Start;
			break;
	}

	switch(TL_State) {
		default:
			break;
	}
}

enum BL_States {BL_Start, BL_Blink, BL_Wait} BL_State;
void Tick_BlinkLED() {
	switch(BL_State) {
		case BL_Start:
			BL_State = BL_Blink;
			break;

		case BL_Blink:
			BL_State = BL_Blink;
			break;

		default:
			BL_State = BL_Start;
			break;
	}

	switch(BL_State) {
		case BL_Blink:
			blinkingLED = ~blinkingLED;
			break;
		
		default:
			break;
	}
}

enum CL_States {CL_Start, CL_Combine} CL_State;
void Tick_CombineLEDs() {
	switch(CL_State) {
		case CL_Start:
			CL_State = CL_Combine;
			break;

		case CL_Combine:
			CL_State = CL_Combine;
			break;

		default:
			CL_State = CL_Start;
			break;
	}

	switch(CL_State) {
		case CL_Combine:
			B = threeLEDs | (blinkingLED << 3) | (speaker << 4);
			break;

		default:
			break;
	}
}

enum S_States {S_Start, S_Init, S_Sound} S_State;
void Tick_Speaker() {
	switch(S_State) {
		case S_Start:
			S_State = S_Init;
			break;

		case S_Init:
			if (A) {
				S_State = S_Sound;
			}
			else {
				S_State = S_Init;
			}
			break;

		case S_Sound:
			if (A) {
				S_State = S_Sound;
			}
			else {
				S_State = S_Init;
			}	
			break;

		default:
			break;
	}

	switch(S_State) {
		case S_Init:
			speaker = 0x01;
			break;

		case S_Sound:
			speaker = ~speaker;
			break;

		default:
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    /* Insert your solution below */
    unsigned long TL_elapsedTime = 150;
    unsigned long BL_elapsedTime = 500;
    const unsigned long timerPeriod = 2;

    TimerSet(timerPeriod);
    TimerOn();

    threeLEDs = 0x01;
    blinkingLED = 0x00;
    //speaker = 0x01;

    TL_State = TL_Start;
    BL_State = BL_Start;
    CL_State = CL_Start;

    while (1) {
	A = ~PINA & 0x04;

	if (TL_elapsedTime >= 150) {
		Tick_ThreeLEDs();
		TL_elapsedTime = 0;
	}
	if (BL_elapsedTime >= 500) {
		Tick_BlinkLED();
		TL_elapsedTime = 0;
	}
	Tick_Speaker();
	Tick_CombineLEDs();
	
	while(!TimerFlag) {}
	TimerFlag = 0;
	TL_elapsedTime += timerPeriod;
	BL_elapsedTime += timerPeriod;

	PORTB = B;
    }
    return 1;
}
