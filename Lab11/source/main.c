/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section:024
 *	Assignment: Lab #7 Exercise #hello world
 *	Exercise Description: hello world LCD program
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "io.h"
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

unsigned short LCD_cnt;
unsigned char LCD_cursorPos;

enum LCD_states {LCD_Init, LCD_Display, LCD_Scroll};

int LCDTick(int state) {
	switch(state) {
		
		case LCD_Init:
			state = LCD_Display;
			break;

		case LCD_Display:
			if (LCD_cursorPos > 0) {
				state = LCD_Scroll;
				LCD_cnt = 0;
			}
			else if (LCD_cursorPos <= 0) {
				state = LCD_Init;
			}
			break;

		case LCD_Scroll:
			if (LCD_cnt < 10) {		// Might need to change time
				state = LCD_Scroll;
			}
			else if (LCD_cnt >= 10) {
				state = LCD_Display;
			}
			break;

		default:
			state = LCD_Init;
			break;
	}

	switch (state) {
		case LCD_Init:
			LCD_cursorPos = 32;
			LCD_ClearScreen();
			LCD_Cursor(LCD_cursorPos);	
			break;

		case LCD_Display:
			LCD_ClearScreen();
			LCD_DisplayString(LCD_cursorPos, "hi");
			break;

		case LCD_Scroll:
			++LCD_cnt;
			--LCD_cursorPos;
			break;			

		default: 
			break;
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
    /* Insert DDR and PORT initializations */
	DDRD = 0xFF; PORTD = 0x00;	// LCD data lines
	DDRB = 0xFF; PORTB = 0x00;	// LCD control lines

	// Initializes the LCD display
	LCD_init();
	
	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	task1.state = start;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct = &LCDTick;

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
