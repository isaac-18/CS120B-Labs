/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: Design a system that reads the 10-bit ADC result 
 *	from the ADC register, and displays the result on a bank of 10 LEDs.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */

    unsigned short s_ADC;
    unsigned char tmpB;
    unsigned char tmpD;

    ADC_init();

    while (1) {
	s_ADC = ADC;
	tmpB = (char)s_ADC;
	tmpD = (char)(s_ADC >> 8);
	
	PORTB = tmpB;
	PORTD = tmpD;
    }
    return 1;
}
