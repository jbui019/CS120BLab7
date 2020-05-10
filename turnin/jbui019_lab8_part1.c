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
#include "io.c"
#endif

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
unsigned char i = 0x00;
unsigned char c = '0';
enum States {Start, INIT, INC, DEC, WAIT, RESET}state;

void tick(){
	switch(state) // transitions
	{ 
		case Start:
			LCD_Cursor(1);
			LCD_WriteData('0');
			state = INIT; 
			break;
		
		
		case INIT:
			if((~PINA & 0x03) == 0x01){
				state = INC; break;
			}
			else if((~PINA & 0x03) == 0x02){
				state = DEC; break;
			}
			else if((~PINA & 0x03) == 0x03){
				state = RESET; break;
			}
			else{
				state = INIT; break;
			}
		
		
		case INC:
		{
			state = WAIT; break;
		}
		
		case DEC:
		{
			state = WAIT; break;
		}
		
		case WAIT:
		{
			if((~PINA & 0x03) == 0x01){
				state = INC; break;
			}
			else if((~PINA & 0x03) == 0x02){
				state = DEC; break;
			}
			else if((~PINA & 0x03) == 0x03){
				state = RESET; break;
			}
			else if((~PINA & 0x03) == 0x00){
				state = INIT; break;
			}
			else {
				state = WAIT; break;
			}
		}
		
		case RESET:
		{
			if((~PINA & 0x03) == 0x00){
				state = INIT; break;
			}
			else{
				state = RESET; break;
			}
		}
		
		default:
			break;
	}
	
	switch(state) //state actions
	{
		case Start:
			break;
			
		case INIT:
			break;
			
		case INC:
		{
			if(i >= 9){
				i = 9;
			}
			else{
				++i;
			}
			LCD_Cursor(1);
			LCD_WriteData(i + '0');
			break;
		}
		
		case DEC:{
			if(i <= 0){
				i = 0;
			}
			else{
				--i;
			}
			LCD_Cursor(1);
			LCD_WriteData(i + '0');
			break;
		}
		
		case WAIT:
			break;
			
		case RESET:
		{
			i = 0; 
			LCD_Cursor(1);
			LCD_WriteData(i + '0');
			break;
		}
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	LCD_init();
	TimerSet(500);
	TimerOn();
    /* Insert your solution below */
    while (1) {
	tick();	
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
