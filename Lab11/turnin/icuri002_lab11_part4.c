/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section:024
 *	Assignment: Lab #11  Exercise #3
 *	Exercise Description: Display character on LCD when pressed on keypad
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "io.h"
#include "bit.h"
#include "timer.h"
#include "keypad.h"

#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

typedef struct task {
	int state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;

unsigned char ch;
unsigned char cursorPos;
unsigned char btnPressed = 1;
unsigned char x = '\0';

enum keypadSM_states {keypadGetBtn};

int keypadSMTick(int state) {
	switch(state) {
		case keypadGetBtn:
			state = keypadGetBtn;
			break;
		default:
			state = keypadGetBtn;
			break;
	}

	switch(state) {
		case keypadGetBtn:
			x = GetKeypadKey();
			break;

		default: break;
	}

	if (x != '\0') {
		ch = x;
	}

	return state;
}

enum LCD_States {LCD_Init, LCD_Wait, LCD_Display};

int LCDTick(int state) {
	switch(state) {
		case LCD_Init:
			state = LCD_Wait;
			break;

		case LCD_Wait:
			state = (x != '\0') ? LCD_Display : LCD_Wait;
			break;

		case LCD_Display:
			if (cursorPos < 16) {
				state = LCD_Wait;
				++cursorPos;
			}
			else if (cursorPos >= 16) {
				state = LCD_Wait;
				cursorPos = 1;
			}
			break;

		default: 
			state = LCD_Init;
			break;
	}

	switch(state) {
		case LCD_Init:
			LCD_DisplayString(1, "Congratulations!");
			LCD_Cursor(1);
			break;

		case LCD_Wait:
			LCD_Cursor(cursorPos);
			break;

		case LCD_Display:
			LCD_WriteData(ch);
			break;

		default: break;
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
	DDRD = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;

	LCD_init();
	LCD_ClearScreen();

	static task task1, task2;
	task *tasks[] = {&task1, &task2 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	task1.state = start;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypadSMTick;

	task2.state = start;
	task2.period = 100;
	task2.elapsedTime = task2.period;
	task2.TickFct = &LCDTick;

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
