/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section:024
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: Play melody upon button press
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

enum States { Start, Init, Play_Melody, Inc_Array, Wait} state;
unsigned char A;
unsigned char i;
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

    if (x == 'C') { set_PWM(toneC); }
    else if (x == 'D') { set_PWM(toneD); }
    else if (x == 'E') { set_PWM(toneE); }
    else if (x == 'F') { set_PWM(toneF); }
    else if (x == 'G') { set_PWM(toneG); }
    else if (x == 'A') { set_PWM(toneA); }
    else if (x == 'B') { set_PWM(toneB); }
    else if (x == 8) { set_PWM(toneC5); }
}

void tick(char* notes, int notesSize, int* noteLength, int noteLenghtSize) {
	switch(state) {
		case Start:
			state = Init;
			break;

		case Init:
			if ((A & 0x01) == 0x01) {
				PWM_on();
				state = Play_Melody;
			}
			break;

		case Play_Melody:
			if (cnt < noteLength[i]) {
				state = Play_Melody;
			}
			else if (cnt >= noteLength[i]) {
				state = Inc_Array;
			}
			break;

		case Inc_Array:
			if (i < notesSize) {
				state = Play_Melody;
			}
			else if (i >= notesSize) {
				PWM_off();
				state = Wait;
			}
			break;

		case Wait:
			if ((A & 0x01) == 0x00) {
				state = Init;
			}
			break;

		default:
			state = Start;
			break;
	}

	switch(state) {

		case Init:
			i = 0;
			cnt = 0;
			break;

		case Play_Melody:
			++cnt;
			playTone(notes[i]);
			break;

		case Inc_Array:
			++i;
			cnt = 0;
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
    PWM_off();

    char notes[] = {'C', 'D', 'C', 'D', 'D', 'E', 'F', 'D', 'E', 'F', 'C'};
    int noteLength[] = {1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1};

    //char notes[] = {'C', 'E', 'C', 'D', 'C', 'D', 'F', 'C', 'D', 'C', 'D', 'C', 'D'};
    //int noteLength[] = {2, 2, 1, 1, 1, 4, 1, 3, 3, 1, 1, 1, 1};

    int notesSize = 11;
    int noteLengthSize = 11;

    /* Insert your solution below */
    while (1) {
	A = ~PINA & 0x01;
	tick(notes, notesSize, noteLength, noteLengthSize);
	while (!TimerFlag) {}
	TimerFlag = 0;
    }
    return 1;
}
