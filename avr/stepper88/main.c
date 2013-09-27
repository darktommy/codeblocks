/*
 */

#include <avr/io.h>
#include "serial.h"

#define OUTPUT PORTC
#define MAX_STEPS 8
#define DELAY 20
#define DELAY_ON_OFF 20
                    //DCBA
//uint8_t steps[] = {0b00000001,0b00000100,0b00000010,0b00001000};
uint8_t steps[] = {0b00000001,0b00000101,0b00000100,0b00000110,0b00000010,0b00001010,0b00001000,0b00001001};
uint8_t current_pos = 0;

void stepF()
{
    OUTPUT = steps[current_pos];
    current_pos++;

    if(current_pos == MAX_STEPS )
        current_pos = 0;

    _delay_ms(DELAY_ON_OFF);

    OUTPUT = 0x00;
}

void stepB()
{
    OUTPUT = steps[current_pos];

    if(current_pos == 0 )
        current_pos = MAX_STEPS - 1;
    else
        current_pos--;

    _delay_ms(DELAY_ON_OFF);

    OUTPUT = 0x00;
}

int main(void)
{
    DDRC = 0xFF;
    OUTPUT = 0x00;

    serialInit(SPEED9600);
    sei();

    uint8_t c;
    uint8_t num;

    serialWrite("hello max!waiting for commands.\n");
    while(1)
    {
        serialWaitUntil('\r');

        c = serialReadChar();
        switch(c)
        {
            case '+':
                num = serialParseInt();
                serialWrite("ok,move forward ");
                serialPrintIntLn(num);

                for(;num>0;num--)
                {
                    stepF();
                    _delay_ms(DELAY);
                }

                break;
            case '-':
                num = serialParseInt();
                serialWrite("ok,move backward ");
                serialPrintIntLn(num);

                for(;num>0;num--)
                {
                    stepB();
                    _delay_ms(DELAY);
                }
                break;

        }
        serialClearBuffer();


    }


    serialEnd();


    return 0;
}
