/*
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "serial.h"

#define OUTPUT PORTC
#define A_CHANNEL PC0
#define B_CHANNEL PC1
#define C_CHANNEL PC2

#define MAX_STEPS 8
#define DELAY 20
#define DELAY_ON_OFF 20

uint8_t outArray[] = {0b100,0b110,0b010,0b011,0b001,0b101,0b100}; //6 базовых векторов

const uint8_t pwmArray1[] PROGMEM= {32,32,31,31,31,30,30,30,29,29,28,28,27,27,27,
26,26,25,25,24,24,23,23,22,22,21,21,20,20,19,18,18,17,17,16,16,15,14,14,13,13,12,
11,11,10,10,9,8,8,7,6,6,5,5,4,3,3,2,1,1}; //массив кодов t1

const uint8_t pwmArray12[] PROGMEM= {32,32,33,33,33,33,34,34,34,34,35,35,35,35,36,
36,36,36,36,36,36,36,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,36,36,36,36,
36,36,36,36,35,35,35,35,34,34,34,34,33,33,33,33,32}; //массив кодов t1+2

uint8_t zeroArray[] = {0, 0, 0, 7, 0, 7, 7};

volatile uint16_t angleIndex;
volatile uint16_t userAngle; /*from 0 to 359 */
volatile uint8_t pwmCounter;

volatile uint8_t t1,t12;
volatile uint8_t v1,v2,v0;

volatile uint8_t userSpeed;
volatile uint8_t direction;

volatile uint8_t rCounter;

void setElectricalAngle(uint16_t);

//прерывание по переполнению счетчика
ISR(TIMER0_OVF_vect)
{
    uint8_t output = OUTPUT;

    if(pwmCounter == 37)
    {
        pwmCounter = 0;
    }

    if (pwmCounter == 0) //цикл начинается, установим всё в 1
    {
        output = v1;
    }

    if(pwmCounter == t1)
    {
        output = v2;
    }

    if(pwmCounter == t12)
    {
        output = v0;
    }


    OUTPUT = output; // выводим всё в порт
    pwmCounter++;
}

ISR(TIMER2_OVF_vect)
{
    if(rCounter == userSpeed)
    {
        userAngle += 5;
        setElectricalAngle(userAngle);
        rCounter = 0;
        return;
    }

    rCounter++;
}


void setElectricalAngle(uint16_t phi)/*phi = [0..359]*/
{

    if (phi>=360)
        phi %= 360;

    uint8_t imod  = phi % 60;
    uint8_t i = phi / 60;

    if(imod == 0) // частный случай, если угол кратен 60
    {
        t1 = 38;//t1 никогда не наступит
        t12 = 32;

        v1 = v2 = outArray[i];
        v0 = 0;

    }
    else // в общем случае
    {
        t1 = pgm_read_byte(&pwmArray1[imod]);
        t12 = pgm_read_byte(&pwmArray12[imod]);

        v1 = outArray[i];
        v2 = outArray[i+1];
        v0 = zeroArray[v2];

    }

    pwmCounter = 0;
    userAngle = phi;
}
/*
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

*/
/*
void setAngle(uint16_t phi)
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
*/
void setupTimer()
{
    //timer0
    pwmCounter = 0;
    userAngle = 0;
    setElectricalAngle(userAngle);

    TIMSK0 = (1<< TOIE0);
    TCNT0 = 0;
    TCCR0A = 0;
    TCCR0B = (1 << CS00);

    userSpeed = 255;
    rCounter = 0;

    TIMSK2 = (1 << TOIE1);
    TCNT2 = 0;
    TCCR2A = 0;
    TCCR2B = (1 << CS20); //prescaler = 1

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

/*
    uint16_t num;
    while(1)
    {
        serialWaitUntil('\r');
        num = serialParseInt();
        serialClearBuffer();
        setElectricalAngle(num);
    }
*/

    serialWrite("hello max!1 - f, 2 - b, 3 ++,4 --\n");
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
                if(userSpeed > 1)
                    userSpeed--;
                break;
            case '4':
                if(userSpeed<255)
                    userSpeed++;
                break;

            }
        }

    }


/*
    uint16_t i=0;
    while(1)
    {

        for(i=0;;i+=1)
        {
            setElectricalAngle(i);
            _delay_ms(2);
        }
    }
*/
    serialEnd();


    return 0;
}
