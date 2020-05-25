/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section:024
 *	Assignment: Lab #10  Exercise #4
 *	Exercise Description: shared SMs that blink one LED in one sec interval and three
 *	others in 300 ms interval. Now includes speaker that sounds on and off every 2ms 
 *	while switch is on. Added buttons to adjust frequency.
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

unsigned char A2;
unsigned char A1;
unsigned char A0;
unsigned char B;

unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char speaker;
unsigned char currFrequency;

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

enum TL_States {TL_Start, TL_LED0, TL_LED1, TL_LED2} TL_State;
void Tick_ThreeLEDs () {
	switch(TL_State) {
		case TL_Start:
			TL_State = TL_LED0;
			break;	

		case TL_LED0:
			TL_State = TL_LED1;
			break;
		
		case TL_LED1:
			TL_State = TL_LED2;
			break;
		
		case TL_LED2:
			TL_State = TL_LED0;
			break;
			
		default:
			TL_State = TL_Start;
			break;
	}

	switch(TL_State) {
		case TL_LED0:
			threeLEDs = 0x01;
			break;
		
		case TL_LED1:
			threeLEDs = 0x02;
			break;
		
		case TL_LED2:
			threeLEDs = 0x04;
			break;
			
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

void Tick_CombineLEDs() {
	B = threeLEDs | ((blinkingLED & 0x01) << 3) | speaker;
}

enum S_States {S_Start, S_Wait, S_On, S_Off} S_State;
void Tick_Speaker() {
	switch(S_State) {
		case S_Start:
			S_State = S_Wait;
			break;

		case S_Wait:
			if (A2) {
				S_State = S_On;
			}
			else {
				S_State = S_Wait;
			}
			break;

		case S_On:
			if (A2) {
				S_State = S_Off;
			}
			else {
				S_State = S_Wait;
			}
			break;

		case S_Off:
			if (A2) {
				S_State = S_On;
			}
			else {
				S_State = S_Wait;
			}
			break;

		default:
			S_State = S_Start;
	}

	switch(S_State) {
		case S_On:
			speaker = 0x10;
			break;
		default:
			speaker = 0x00;
			break;
	}
}

enum CF_States {CF_Start, CF_Wait, CF_Inc, CF_Dec} CF_State;
void Tick_ChangeFreq() {
	switch(CF_State) {
		case CF_Start:
			CF_State = CF_Wait;
			break;

		case CF_Wait:
			if (A0) {
				CF_State = CF_Inc;
			}
			else if (A1) {
				CF_State = CF_Dec;
			}
			break;

		case CF_Inc:
			CF_State = CF_Wait;
			break;

		case CF_Dec:
			CF_State = CF_Wait;
			break;

		default:
			CF_State = CF_Start;
			break;
	}

	switch(CF_State) {
		case CF_Inc:
			++currFrequency;
			break;

		case CF_Dec:
			if (currFrequency > 1) {
				--currFrequency;
			}
			break;

		default:
			break;
	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    threeLEDs = 0x01;
    blinkingLED = 0x00;
    currFrequency = 2;
    
    unsigned long TL_elapsedTime = 300;
    unsigned long BL_elapsedTime = 1000;
    unsigned long S_elapsedTime = currFrequency;
    unsigned long CF_elapsedTime = 100;
    const unsigned long timerPeriod = 1;

    TimerSet(timerPeriod);
    TimerOn();

    TL_State = TL_Start;
    BL_State = BL_Start;
    S_State = S_Start;

    while (1) {
	A2 = ~PINA & 0x04;
	A1 = ~PINA & 0x02;
	A0 = ~PINA & 0x01;

	if (TL_elapsedTime >= 300) {
		Tick_ThreeLEDs();
		TL_elapsedTime = 0;
	}
	if (BL_elapsedTime >= 1000) {
		Tick_BlinkLED();
		BL_elapsedTime = 0;
	}
	if (CF_elapsedTime >= 100) {
		Tick_ChangeFreq();
		CF_elapsedTime = 0;
	}
	if (S_elapsedTime >= currFrequency) {	
		Tick_Speaker();
		S_elapsedTime = 0;
	}
	Tick_CombineLEDs();
	
	while(!TimerFlag) {}
	TimerFlag = 0;
	TL_elapsedTime += timerPeriod;
	BL_elapsedTime += timerPeriod;
	S_elapsedTime += timerPeriod;
	CF_elapsedTime += timerPeriod;

	PORTB = B;
    }
    return 1;
}
