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

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	
	_avr_timer_cntcurr = _avr_timer_M;
	
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;	
}

void TimerISR(){
	TimerFlag = 1;	
}

void ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
void tick(){
	unsigned char PB = PORTC;
	unsigned char flag = 0;
	unsigned char A1 = ~PINA & 0x02;
	unsigned char A0 = ~PINA & 0x01;
	
	if (A0 && A1){ 
		state = reset;
	}
	else {
		switch(state){
		 
		case init:
			if(A0){
				state = A0press;
			}
			else if(A1){
				state = A1press;
			}
			else{
				state = INIT;
			}
			break;
				
		case A0press:
			counter = 0;
			if(A0){
				state = A0remain;
			}
			else if(!A0){
				state = A0release;
			}
			else{
				state = A0remain;
			}
			break;
				
		case A1press:
			counter = 0;
			if(A1) state = A1remain;
			else if(!A1) state = A1release;
			else state = A1remain;
			break;
				
		case A0remain:
			if(A0){
				state = A0remain;
			}
			else{
				state = A0release;
			}
			break;
				
		case A1remain:
			if(A1){
				state = A1remain;
			}
			else{
				state = A1release;
			}
			break;
				
		case A0release:
			counter = 0;
			if(A0){
				state = A0press;
			}
			else if(A1){
				state = A1press;
			}
			else{
				state = A0release;
			}
			break;
		case A1release:
			counter = 0;
			if(A0){
				state = A0press;
			}
			else if(A1){
				state = A1press;
			}
			else{
				state = A1release;
			}
			break;
		case reset:
			if(A0){
				state = A0press;
			}
			else if(A1){
				state = A1press;
			}
			else{
				state = reset;
			}
			break;
				
		default:
			state = init;
			break; 
		}
	}
	
	switch(state){
	case init:
		PORTC = 7;
		break;
	case A0press:
		if(PB < 9){
			PORTC = PB + 1;
		}
		break;
	case A0remain:
		counter++;
		if(counter == 100 && PB < 9){
			PORTC = PB+1;
			counter = 0;
		}
		break;
	case A1press:
		if(PB > 0){
			PORTC = PB -1;
		}
		break;
	case A1remain:
		counter++;
		if(counter == 100 && PB > 0)
		{
			PORTC = PB-1;
			counter = 0;
		}
		break;
	case reset:
		PORTC = 0;
		break;
	default:
		break;
	}

}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	state = init;
	TimerSet(1);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	tick();	
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
