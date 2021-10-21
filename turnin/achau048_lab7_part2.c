/*  Author: Abdullah Chaudhry
 *  Partner(s) Name: 
 *  Lab Section: 021
 *  Assignment: Lab #7 Exercise #2
 *  Exercise Description: [optional - include for your own benefit]
 *
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 *  
 *  Demo Link: https://drive.google.com/file/d/1YrxUY8aMnf36NzsgWTuROwvDFG8yDjJ9/view?usp=sharing (Adjust quality in bottom right to 1080p to see LCD)
 *
 */

#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "io.h"

enum States {Start, pb1On, pb2On, pb3On, button, ledWait, victory} state;
unsigned char score = 5;
unsigned char victoryFlag = 0;

void Tick(){
    switch(state){ //State Transitions
        case Start:
            state = pb1On;
            score = 5;
            victoryFlag = 0;
            break;

        case pb1On:
            if(PINA == 0xFE){
                state = button;
            }else{
                state = pb2On;
            }
            break;

        case pb2On:
            if(PINA == 0xFE){
                state = button;
            }else{
                state = pb3On;
            }            
            break;

        case pb3On:
            if(PINA == 0xFE){
                state = button;
            }else{
                state = pb1On;
            }
            break;

        case button:
            if(PORTB == 0x02){
                score++;
            }else if(score > 0){
                score--;
            }

            if(score == 9){
                state = victory;
                break;
            }

            if(PINA == 0xFE){
                state = button;
            }else{
                state = ledWait;
            }
            break;

        case ledWait:
            if(PINA == 0xFE){
                state = pb1On;
            }else{
                state = ledWait;
            }
            break;

        case victory:
            break;

        default:
            state = pb1On;
            break;
    }

    switch(state){ //State Actions
        case Start:
            break;

        case pb1On:
            PORTB &= 0xF8; //BBBB B000
            PORTB |= 0x01; //BBBB B001
            break;

        case pb2On:
            PORTB &= 0xF8; //BBBB B000
            PORTB |= 0x02; //BBBB BB10
            break;

        case pb3On:
            PORTB &= 0xF8; //BBBB B000
            PORTB |= 0x04; //BBBB B100
            break;

        case button:
            break;

        case ledWait:
            break;

        case victory:
            victoryFlag = 1;
            LCD_DisplayString(1, "VICTORY SCREECH!");
            if(PINA == 0xFE){
                LCD_ClearScreen();
                state = Start;
            }
            break;

        default:
            PORTB &= 0xF8; //BBBB B000
            PORTB |= 0x01; //BBBB B001
            break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PINA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    /* Insert your solution below */
    TimerSet(300);
    TimerOn();
    LCD_init();

    while (1) {
        Tick();
        if(!victoryFlag){
            LCD_Cursor(1);
            LCD_WriteData(score + 48);
        }
        while(!TimerFlag) {}; //Wait 1 second
        TimerFlag = 0;
    }
    return 1;
}