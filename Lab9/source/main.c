/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section:024
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
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

enum States { Start, OFF, ON, Wait, Increment, Decrement } state;
unsigned char A;
unsigned char cnt;

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

void set_PWM(double frequency) {
	static double current_frequency;

	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; } 

		if (frequency < 0.954) { OCR3A = 0xFFFF; }

		else if (frequency > 31250) { OCR3A = 0x0000; }

		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);

	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

void playTone(unsigned char x) {	
    unsigned short toneC = 261.63;
    unsigned short toneD = 293.66;
    unsigned short toneE = 329.63;
    unsigned short toneF = 349.23;
    unsigned short toneG = 392.00;
    unsigned short toneA = 440.00;
    unsigned short toneB = 493.88;
    unsigned short toneC5 = 523.25;

    if (x == 1) { set_PWM(toneC); }
    else if (x == 2) { set_PWM(toneD); }
    else if (x == 3) { set_PWM(toneE); }
    else if (x == 4) { set_PWM(toneF); }
    else if (x == 5) { set_PWM(toneG); }
    else if (x == 6) { set_PWM(toneA); }
    else if (x == 7) { set_PWM(toneB); }
    else if (x == 8) { set_PWM(toneC5); }
}

void tick() {
	switch(state) {
		case Start:
			state = OFF;
			break;

		case OFF:
			if ((A & 0x01) == 0x01) {
				state = ON;
			}
			break;

		case ON:
			if ((A & 0x01) == 0x00) {
				state = Wait;
			}
			break;

		case Wait:
			if ((A & 0x02) == 0x02) {
				state = Increment;
			}
			else if ((A & 0x04) == 0x04) {
				state = Decrement;
			}
			else if ((A & 0x01) == 0x01) {
				state = OFF;
			}
			break;

		case Increment:
			state = Wait;
			break;

		case Decrement:
			state = Wait;
			break;

		default:
			state = Start;
			break;
	}

	switch(state) {
		case OFF:
			PWM_off();
			cnt = 1;
			break;

		case ON:
			PWM_on();
			//++cnt;
			//playTone(cnt);
			break;

		case Increment:
			if (cnt < 8) {
				cnt = cnt + 1;
			}
			//playTone(cnt);
			break;

		case Decrement:
			if (cnt > 1) {
				cnt = cnt - 1;
			}
			//playTone(cnt);
			break;

		default:
			break;

	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    TimerSet(100);
    TimerOn();

    state = Start;
    cnt = 1;
    PWM_off();

    /* Insert your solution below */
    while (1) {
	A = ~PINA & 0x07;
	tick();
	playTone(cnt);
	while (!TimerFlag) {}
	TimerFlag = 0;
    }
    return 1;
}
