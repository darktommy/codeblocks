/*
 */

#include <avr/io.h>
#include "serial.h"

#define DELAY_STEP 6
#define DELAY_ON_OFF 1

#define PORT_X PORTB
#define PORT_Y PORTC

#define AXIS_Z_A PD6
#define AXIS_Z_B PD7
#define PORT_Z PORTD

#define ENDING_PORT PORTD
#define ENDING_X PD3
#define ENDING_Y PD2
#define ENDING_Z PD4
                    //DCBA
//uint8_t steps[] = {0b00000001,0b00000100,0b00000010,0b00001000};
#define MAX_STEPS 8
uint8_t steps[] = {0b00000001,0b00000101,0b00000100,0b00000110,0b00000010,0b00001010,0b00001000,0b00001001};
volatile uint8_t current_step_X = 0;
volatile uint8_t current_step_Y = 0;

volatile uint8_t current_pos_X = 0;
volatile uint8_t current_pos_Y = 0;
volatile uint8_t current_pos_Z = 0;

volatile uint8_t isEndX = 0;
volatile uint8_t isEndY = 0;
volatile uint8_t isEndZ = 0;


uint8_t check_ending_X()
{
    uint8_t isEnd = PIND & (0b00001000);
    return ((isEndX = isEnd) == 0);
}

uint8_t check_ending_Y()
{
    uint8_t isEnd = PIND & (0b00000100);
    return ((isEndY = isEnd) == 0);
}

uint8_t check_ending_Z()
{
    uint8_t isEnd = PIND & (0b00010000);
    return ((isEndZ = isEnd) == 0);
}


void stepF_X()
{
    cli();
    PORT_X = (PORT_X & (0b11110000)) | steps[current_step_X];
    current_step_X++;

    current_pos_X++;

    if(current_step_X == MAX_STEPS )
        current_step_X = 0;

    _delay_ms(DELAY_STEP);

    PORT_X = 0x00;
    sei();
}

void stepB_X()
{
    cli();
    PORT_X = (PORT_X & (0b11110000)) | steps[current_step_X];

    current_pos_X--;

    if(current_step_X == 0 )
        current_step_X = MAX_STEPS - 1;
    else
        current_step_X--;

    _delay_ms(DELAY_STEP);

    PORT_X = 0x00;
    sei();
}

void stepF_Y()
{
    cli();
    PORT_Y = (PORT_Y & (0b11110000)) | steps[current_step_Y];
    current_step_Y++;
    current_pos_Y++;

    if(current_step_Y == MAX_STEPS )
        current_step_Y = 0;

    _delay_ms(DELAY_STEP);

    PORT_Y = 0x00;
    sei();
}

void stepB_Y()
{
    cli();
    PORT_Y = (PORT_Y & (0b11110000)) | steps[current_step_Y];

    current_pos_Y--;

    if(current_step_Y == 0 )
        current_step_Y = MAX_STEPS - 1;
    else
        current_step_Y--;

    _delay_ms(DELAY_STEP);

    PORT_Y = 0x00;
    sei();
}

void stepF_Z()
{
    PORT_Z |= 0b10000000;
    _delay_ms(DELAY_ON_OFF);
    PORT_Z &= ~(0b10000000);
    current_pos_Z++;
}

void stepB_Z()
{
    PORT_Z |= 0b01000000;
    _delay_ms(DELAY_ON_OFF);
    PORT_Z &= ~(0b01000000);
    current_pos_Z--;
}


void returnX()
{
    while(!check_ending_X())
    {
        stepB_X();
    }
    current_pos_X = 0;
}

void returnY()
{
    while(!check_ending_Y())
    {
        stepB_Y();
    }
    current_pos_Y = 0;
}

void returnZ()
{
    while(!check_ending_Z())
    {
        stepB_Z();
        _delay_ms(2);
    }
    current_pos_Z = 0;
}

void InitPORTS()
{
    //       76543210
    DDRC = 0b00001111;
    DDRB = 0b00001111;
    DDRD = 0b11000000;

    PORT_X = 0x00;
    PORT_Y = 0x00;
    //         76543210
    PORT_Z = 0b00011100; // PD6,7 --  в ноль; PD4,3,2 -- pull-up

}

int main(void)
{
    InitPORTS();
    serialInit(SPEED9600);
    sei();

    void (*func_F)(void) = 0;
    void (*func_B)(void) = 0;
    void (*func_ret)(void) = 0;

    uint8_t c;
    uint8_t num;

    serialWrite("hello max!waiting for commands.\n");
    while(1)
    {
        serialWaitUntil('\r');
        c = serialReadChar();
        switch(c)
        {
        case 'X':
        case 'x':
            func_F = stepF_X;
            func_B = stepB_X;
            func_ret = returnX;
            break;
        case 'Y':
        case 'y':
            func_F = stepF_Y;
            func_B = stepB_Y;
            func_ret = returnY;
            break;
        case 'Z':
        case 'z':
            func_F = &stepF_Z;
            func_B = &stepB_Z;
            func_ret = returnZ;
            break;
        }

        c = serialReadChar();
        switch(c)
        {
            case '+':
                num = serialParseInt();
                for(;num>0;num--)
                {
                    func_F();
                }
                serialWrite("ok\n");
                break;
            case '-':
                num = serialParseInt();
                for(;num>0;num--)
                {
                    if (func_B == stepB_X )
                    {

                        if(check_ending_X()){
                        serialWrite("End of X reached\n");
                        break;
                        }
                    }
                    else if (func_B == stepB_Y )
                    {
                        if(check_ending_Y()){
                            serialWrite("End of Y reached\n");
                            break;
                            }
                    }
                    else if (func_B == stepB_Z )
                    {
                        if(check_ending_Z()){
                            serialWrite("End of Z reached\n");
                            break;
                            }
                    }
                    func_B();
                }
                serialWrite("ok\n");
                break;
            case '0':
                func_ret();
                serialWrite("ok\n");
                break;
        }
        serialClearBuffer();
    }

    serialEnd();


    return 0;
}
