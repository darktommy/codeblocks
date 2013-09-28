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

uint8_t vectA[]={220, 239, 250, 254, 250, 239, 220, 195, 163, 127, 87, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 87, 127,163, 195, 220, 239, 250, 254, 250, 239, 220};
uint8_t vectB[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 87, 127, 163, 195, 220, 239, 250, 254, 250, 239, 220, 239, 250, 254,250, 239, 220, 195, 163, 127, 87, 44, 0};
uint8_t vectC[]={0, 44, 87, 127, 163, 195, 220, 239, 250, 254, 250, 239, 220, 239, 250, 254, 250, 239, 220, 195, 163, 127, 87, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

volatile uint8_t A,B,C;

volatile uint16_t angleIndex;
volatile uint16_t userAngle; /*from 0 to 1440 */
volatile uint8_t pwmCounter;

volatile uint8_t userDelay;
volatile uint8_t direction;

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


void setElectricalAngle(uint16_t phi)/*phi = [0..359]*/
{
    uint8_t i;
    if (phi>=360)
        phi %= 360;

    angleIndex = phi / 10;

    //интерполяция:
    if((i = phi % 10) == 0)
    {
        A = vectA[angleIndex];
        B = vectB[angleIndex];
        C = vectC[angleIndex];
    }
    else
    {
        uint8_t A1,B1,C1,A2,B2,C2;

        A1 = vectA[angleIndex];
        B1 = vectB[angleIndex];
        C1 = vectC[angleIndex];

        A2 = vectA[angleIndex+1];
        B2 = vectB[angleIndex+1];
        C2 = vectC[angleIndex+1];

        A = A1 + (uint8_t)((A2-A1)*((float)i/10));
        B = B1 + (uint8_t)((B2-B1)*((float)i/10));
        C = C1 + (uint8_t)((C2-C1)*((float)i/10));

    }
}

void goCCW(uint16_t phi)
{
    volatile int16_t tmp;
    if(phi < userAngle)
        phi+=1440; //на случай, если переходим через 0

    if(phi - userAngle > 120)
    {
        for(tmp = userAngle+120;tmp<phi;tmp+=120)
        {
            setElectricalAngle(tmp);
            _delay_ms(20);
        }

    }

    setElectricalAngle(phi);
    userAngle = phi;
    if(userAngle >= 1440) userAngle-=1440;

}

void goCW(uint16_t phi)
{
    volatile int16_t tmp;
    if(userAngle < phi)
        userAngle+=1440; //на случай, если переходим через 0

    tmp = userAngle;
    while(tmp - phi  >= 120)
    {
        tmp-=120;
        setElectricalAngle(tmp);
        _delay_ms(20);
    }

    setElectricalAngle(phi);
    userAngle = phi;
}


void setAngle(uint16_t phi)/*phi = [0..1440]*/
{
    if(userAngle > phi)
    {
        if(userAngle - phi > 720)
            goCCW(phi);
        else
            goCW(phi);
    }
    else
    {
        if(phi - userAngle > 720)
            goCW(phi);
        else
            goCCW(phi);
    }

}

void setupTimer()
{
    //timer0
    pwmCounter = 0;
    userAngle = 0;
    setAngle(userAngle);

    TIMSK0 = (1<< TOIE0);
    TCNT0 = 0;
    TCCR0A = 0;
    TCCR0B = (1 << CS00);

    userDelay = 100;

}

void stepF()
{
}

void stepB()
{
}


int main(void)
{
    DDRC = 0xFF;
    OUTPUT = 0x00;

    serialInit(SPEED9600);
    setupTimer();
    sei();


    uint16_t num;

    while(1)
    {
        serialWaitUntil('\r');
        num = serialParseInt();
        serialClearBuffer();
        setAngle(num);
    }

/*
    serialWrite("hello max!1 - f, 2 - b, 3 ++,4 --,5 - man/auto \n");
    while(1)
    {
        if(serialAvailable()>0)
        {
            uint8_t c = serialReadChar();

            switch(c)
            {
            case '1':
                if(direction)
                    direction = 1;
                break;
            case '2':
                if(direction)
                    direction = 2;
                break;
            case '3':
                if(userDelay>10)
                    userDelay--;
                break;
            case '4':
                if(userDelay<100)
                    userDelay++;
                break;
            case '5':
                if(direction)
                    direction = 0;
                else
                    direction = 1;
                break;

            }
        }
        else
        {
            switch(direction)
            {
            case 0:
                break;
            case 1:
                userAngle++;
                setAngle(userAngle);
                _delay_ms(userDelay);
                break;
            case 2:
                userAngle--;
                setAngle(userAngle);
                _delay_ms(userDelay);
                break;
            }
        }


    }
*/

/*
    uint16_t i=0;
    while(1)
    {

        for(i=0;;i+=60)
        {
            setAngle(i);
            _delay_ms(500);
        }
    }
*/
    serialEnd();


    return 0;
}
