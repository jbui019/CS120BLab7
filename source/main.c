/*	Author: jbui019
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{start, d1, d2, d3}state;
volatile unsigned char TimerFlag = 0;
void TimerISR(){ TimerFlag = 1;}

void tick(){
	switch(state){//transitions
		case start:
			state = d1;
			break;

		case d1:
			state = d2;
			break;
		
		case d2:
			state = d3;
			break;

		case d3:
			state = d1;	
		default:
			state =  start;
			break;

	}
	
	switch(state){//actions
		case start:
			break;

		case d1:
			PORTB = 0x01;
			break;

		case d2:
			PORTB = 0x02;
			break;

		case d3: 
			PORTB = 0x04;
			break;
	
	}	
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	tick();	
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
