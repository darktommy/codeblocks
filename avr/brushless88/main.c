/*
 */

#include <avr/io.h>
#include "serial.h"

#define OUTPUT PORTC
#define A_CHANNEL PC0
#define B_CHANNEL PC1
#define C_CHANNEL PC2

#define MAX_STEPS 8
#define DELAY 20
#define DELAY_ON_OFF 20
                    //DCBA
//uint8_t steps[] = {0b00000001,0b00000100,0b00000010,0b00001000};
uint8_t steps[] = {0b00000001,0b00000101,0b00000100,0b00000110,0b00000010,0b00001010,0b00001000,0b00001001};
uint8_t current_pos = 0;


uint8_t vectA[]={127, 163, 195, 220, 239, 250, 254, 250, 239, 220, 239, 250, 254, 250, 239, 220, 195, 163, 127, 87, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 87, 127};
uint8_t vectB[]={254, 250, 239, 220, 195, 163, 127, 87, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 87, 127, 163, 195, 220, 239, 250, 254, 250, 239, 220, 239, 250, 254};
uint8_t vectC[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 87, 127, 163, 195, 220, 239, 250, 254, 250, 239, 220, 239, 250, 254, 250, 239, 220, 195, 163, 127, 87, 44, 0, 0, 0, 0};

volatile uint8_t A,B,C;

volatile uint16_t angleIndex;
volatile uint16_t userAngle;
volatile uint8_t pwmCounter;

//прерывание по переполнению счетчика
ISR(TIMER0_OVF_vect)
{
    uint8_t output = OUTPUT;

    if (pwmCounter == 0) //цикл начинается, установим всё в 1
    {
        output |= (1 << A_CHANNEL) | (1<< B_CHANNEL) | (1 << C_CHANNEL);
    }

    if (pwmCounter == A)
    {
        output &= ~(1 << A_CHANNEL);
    }
    if (pwmCounter == B)
    {
        output &= ~(1 << B_CHANNEL);
    }
    if (pwmCounter == C)
    {
        output &= ~(1 << C_CHANNEL);
    }

    OUTPUT = output; // выводим всё в порт

    pwmCounter++;

}

void setAngle(uint16_t phi)
{
    userAngle = phi;
    if (userAngle>=360)
        userAngle %= 360;

    angleIndex = userAngle / 10;

    A = vectA[angleIndex];
    B = vectB[angleIndex];
    C = vectC[angleIndex];
}

void setupTimer()
{
    pwmCounter = 0;
    userAngle = 0;
    setAngle(userAngle);

    TIMSK0 = (1<< TOIE0);
    TCNT0 = 0;
    TCCR0A = 0;
    TCCR0B = (1 << CS00);

}

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
    setupTimer();
    sei();


    uint16_t num;

    serialWrite("hello max!enter angle.\n");
    while(1)
    {
        serialWaitUntil('\r');
        num = serialParseInt();
        serialClearBuffer();

        setAngle(num);

        serialWrite("angleIndex:");
        serialPrintIntLn(angleIndex);






    }


    serialEnd();


    return 0;
}
