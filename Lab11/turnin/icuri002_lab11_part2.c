/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section:024
 *	Assignment: Lab #11 Exercise #2
 *	Exercise Description: paginate/scroll text on LCD
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
unsigned char LCD_stateTimer = 2;

enum LCD_states {LCD_Init, LCD_Display1, LCD_Display2, LCD_Display3, LCD_Wait};

int LCDTick(int state) {
	switch(state) {
		
		case LCD_Init:
			state = LCD_Display1;
			break;

		case LCD_Display1:					
			if (LCD_cnt < LCD_stateTimer) {		// Might need to change time
				state = LCD_Display1;
			}
			else if (LCD_cnt >= LCD_stateTimer) {
				state = LCD_Display2;
				LCD_ClearScreen();
				LCD_cnt = 0;
			}
			break;

		case LCD_Display2:					
			if (LCD_cnt < LCD_stateTimer) {		// Might need to change time
				state = LCD_Display2;
			}
			else if (LCD_cnt >= LCD_stateTimer) {
				state = LCD_Display3;
				LCD_ClearScreen();
				LCD_cnt = 0;
			}
			break;

		case LCD_Display3:					
			if (LCD_cnt < LCD_stateTimer) {		// Might need to change time
				state = LCD_Display3;
			}
			else if (LCD_cnt >= LCD_stateTimer) {
				state = LCD_Display1;
				LCD_ClearScreen();
				LCD_cnt = 0;
			}
			break;

		default:
			state = LCD_Init;
			break;
	}

	switch (state) {
		case LCD_Init:
			LCD_ClearScreen();
			break;

		case LCD_Display1:
			++LCD_cnt;
			LCD_DisplayString(1, "CS120B is Legend...");
			break;

		case LCD_Display2:
			++LCD_cnt;
			LCD_DisplayString(1, "wait for it");
			break;

		case LCD_Display3:
			++LCD_cnt;
			LCD_DisplayString(1, "DARY!");
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
	task1.period = 1000;
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
