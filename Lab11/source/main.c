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
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRD = 0xFF; PORTD = 0x00;	// LCD data lines
	DDRB = 0xFF; PORTB = 0x00;	// LCD control lines

	// Initializes the LCD display
	LCD_init();

	// Starting position 1 on the LCD screen, writes Hello World
	LCD_DisplayString(1, "Fuck SMs");

    /* Insert your solution below */
    while (1) {
	continue;
    }
    return 1;
}
