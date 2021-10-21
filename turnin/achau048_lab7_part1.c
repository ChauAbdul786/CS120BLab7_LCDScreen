/*  Author: Abdullah Chaudhry
 *  Partner(s) Name: 
 *  Lab Section: 021
 *  Assignment: Lab #7 Exercise #1
 *  Exercise Description: [optional - include for your own benefit]
 *
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 *  
 *  Demo Link: https://drive.google.com/file/d/1GuOfLQXAA3P8uVyyOIsWhG3_bhbvF-tM/view?usp=sharing
 *
 */

#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "io.h"

enum States {Start, state} State; 
unsigned char tmpA;
unsigned char count = 7;

void Tick(){
    switch(State){ //State Transitions
        case Start:
            State = state;
            count = 0x07;
            break;

        case state:
            if((tmpA & 0x03) == 0x03){
                //Do nothing
            }else if((tmpA & 0x03) == 0x02){
                if(count < 9){
                    count++;
                }
            }else if((tmpA & 0x03) == 0x01){
                if(count > 0){
                    count--;
                }
            }else if((tmpA & 0x03) == 0x00){
                count = 0;
            }
            break;

        default:
            State = state;
            break;
    }
    //No State Actions
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PINA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    LCD_init();

    /* Insert your solution below */
    TimerSet(1000);
    TimerOn();

    while (1) {
        tmpA = PINA;
        Tick();
        LCD_Cursor(1);
        LCD_WriteData(count + 48);
        while(!TimerFlag) {}; //Wait 1 second
        TimerFlag = 0;
    }
    return 1;
}