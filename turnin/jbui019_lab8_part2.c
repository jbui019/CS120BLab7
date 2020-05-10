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

enum States{start, sequence, press, waitRelease, stay, reset }state;
volatile unsigned char TimerFlag = 0;
unsigned char A;
unsigned char tmpB = 0x00;
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
unsigned char flag = 0;
void tick(){
switch(state){
		case start:
			state = sequence;
			break;
		case sequence:
			if(A) {
				state = press;
			}
			else{
				state = sequence; 
			}
			TimerFlag = 0;
			break;
		case press:
			if(A){ 
				state = waitRelease;
			}
			else if(!flag){ 
				state = stay;
			}
			else if(flag){ 
				state = reset;
			}
			break;
		case waitRelease:
			if(A){
				state = waitRelease;
			}
			else if(flag){
				state = stay;
			}
			else if(!flag){ 
				state = reset;
			}
			break;
		case stay:
			if(A){ 
				state = press;
			}
			else{ 
				state = stay;
			}
			break;
		case reset:
			state = start;
			break;
		default:
			state = start;
			break;
	}
	
	switch(state){
		case start:
			tmpB = 0x00;
			break;
		case sequence:
			if(tmpB == 0 || tmpB == 4){
				tmpB = 1;
			}
			else if(tmpB == 1){
				tmpB = 2;
			}
			else if(tmpB == 2){
				tmpB = 4;	
			}
			break;
		case press:
			flag = !flag;
			break;
		case waitRelease:
			break;
		case stay:
			break;
		case reset:
			tmpB = 0x00;
			break;
		default:
			break;
		}
		PORTB = tmpB;
		
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = start;
	TimerSet(15);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	A = ~PINA & 0x01;
	tick();	
	while(!TimerFlag){
		A = ~PINA & 0x01;
		if(A){tick();}
	}
	TimerFlag = 0;
    }
    return 1;
}
