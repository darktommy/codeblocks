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
#define MAX_STEPS 4
uint8_t steps[] = { 0b0001,
                    0b0010,
                    0b0100,
                    0b1000};

#define MICROSTEPS 16
volatile uint8_t microstep = 0;

const uint8_t SineTable32[] = {
/*
 0, 23, 48, 73, 96, 119, 140, 160, 179, 196, 211, 223, 234, 243, 249, 252,
 254, 252, 249, 243, 234, 223, 211, 196, 179, 160, 140, 119, 96, 73, 48, 23
*/
   0,25,50,74,98,120,142,162,180,197,212,225,236,244,250,254,
  255,254,250,244,236,225,212,197,180,162,142,120,98,74,50,	25
 };

volatile uint8_t current_step_X = 0;
volatile uint8_t current_step_Y = 0;

volatile uint8_t current_pos_X = 0;
volatile uint8_t current_pos_Y = 0;
volatile uint8_t current_pos_Z = 0;

volatile uint8_t isEndX = 0;
volatile uint8_t isEndY = 0;
volatile uint8_t isEndZ = 0;

ISR(TIMER0_OVF_vect)
{
/*
    OCR0A = SineTable32[(microstep+MICROSTEPS)%(MICROSTEPS*2)];
    OCR0B = SineTable32[microstep];
    PORT_X = steps[current_step_X] | steps[(current_step_X+1) % MAX_STEPS];
*/
/*
    OCR0A = 255-SineTable32[(microstep+MICROSTEPS)%(MICROSTEPS*2)];
    OCR0B = 255-SineTable32[microstep];
    PORT_X = 0;
*/
    OCR0A = SineTable32[(microstep+MICROSTEPS)%(MICROSTEPS*2)];
    OCR0B = 255-SineTable32[microstep];
    PORT_X = steps[current_step_X];
}

ISR(TIMER0_COMPA_vect)
{
    //OCR0A = SineTable32[(microstep+MICROSTEPS)%(MICROSTEPS*2)];
    PORT_X &= ~(steps[current_step_X]);
    //PORT_X |= steps[current_step_X];
}

ISR(TIMER0_COMPB_vect)
{
    //OCR0B = SineTable32[microstep];
    //PORT_X &= ~steps[(current_step_X+1) % MAX_STEPS];
    PORT_X |= steps[(current_step_X+1) % MAX_STEPS];
}


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

    TCCR0A = 0x00;
    TCNT0 = 0;
    OCR0A = 0;
    OCR0B = 0;
    TCCR0B = (1 << CS01) | (1 << CS00);
    TIMSK0 = (1 << TOIE0) | (1 << OCIE0A) | (1 << OCIE0B);


    sei();
    while(1)
    {
/*
        if (serialAvailable() > 0)
        {
            if(serialReadChar() == '+')
                microstep++;
            if(microstep == MICROSTEPS)
            {
                microstep = 0;
                current_step_X++;
            }
            if(current_step_X == MAX_STEPS)
            {
                current_step_X = 0;
            }
        }*/


        microstep++;
        if(microstep == MICROSTEPS)
        {
            microstep = 0;
            current_step_X++;
        }
        if(current_step_X == MAX_STEPS)
        {
            current_step_X = 0;
        }
        _delay_ms(50);
    }

    return 0;
}
