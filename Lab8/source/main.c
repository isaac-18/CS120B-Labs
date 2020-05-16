/*	Author: icuri002
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #8  Exercise #3
 *	Exercise Description: Design a system where an LED is illuminated only if enough light
 *	is detected from the photo resistor.
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
    /* Insert your solution below */

    unsigned short s_ADC;
    unsigned char tmpB;
    unsigned short MAX = 112;
    unsigned short MIN = 31;

    ADC_init();

    while (1) {
	s_ADC = ADC;

	if (s_ADC >= (MAX / 2)) {
		tmpB = 0x01;
	} 
	else {
		tmpB = 0x00;
	}
	PORTB = tmpB;
    }
    return 1;
}
