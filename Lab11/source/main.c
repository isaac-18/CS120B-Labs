/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section:024
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: Modified keypad code to be in an SM Task and utilize
 *			      task scheduler.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "bit.h"
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

typedef struct task {
	int state;		// change to int? int state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;


unsigned char GetKeypadKey() {
	PORTC = 0xEF;
	asm("nop");
	if (GetBit(PINC, 0) == 0) { return('1'); }
	if (GetBit(PINC, 1) == 0) { return('4'); }
	if (GetBit(PINC, 2) == 0) { return('7'); }
	if (GetBit(PINC, 3) == 0) { return('*'); }

	PORTC = 0xDF;
	asm("nop");
	if (GetBit(PINC, 0) == 0) { return('2'); }

	PORTC = 0xBF;
	asm("nop");

	return('\0');
}

enum keypadSM_states {keypadWait, keypad1, keypad2, keypadStar};

int keypadSMTick(int state) {
	unsigned char x = GetKeypadKey();

	switch(state) {
		case keypadWait:
			if (x == '1') {
				state = keypad1;
			}
			else if (x == '2') {
				state = keypad2;
			}
			else if (x == '*') {
				state = keypadStar;
			}
			break;

		default:
			state = keypadWait;
			break;
	}


	switch (state) {
	//	case '\0': PORTB = 0x1F; break;
	
		case keypad1: PORTB = 0x01; break;
		case keypad2: PORTB = 0x02; break;
		case keypadStar: PORTB = 0x0E; break;
		case keypadWait: break;

	/*	case 'D': PORTB = 0x0D; break;
		case '*': PORTB = 0x0E; break;
		case '0': PORTB = 0x00; break;
		case '#': PORTB = 0x1B; break;
	*/
		default: PORTB = 0x01B; break;	// Should never occur. Middle LED off.
	}

	return state;
}

unsigned long int findGCD(unsigned long int a, unsigned long int b) {
	unsigned long int c;
	while(1) {
		c = a % b;
		if (c == 0) { return b; }
		a = b;
		b = c;
	}
	return 0;
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;

	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	task1.state = start;
	task1.period = 1;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypadSMTick;

	unsigned long int GCD = tasks[0]->period;
	for (unsigned int i = 0; i < numTasks; ++i) {
		GCD = findGCD(GCD, tasks[i]->period);
	}

	TimerSet(GCD);
	TimerOn();

	unsigned short i;

	while (1) {
		for (i = 0; i < numTasks; ++i)  {
			if (tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += GCD;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    	}
   	return 0;
}
