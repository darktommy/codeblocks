/*
 */

#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "mirf.h"

#define DIR_FWD 1
#define DIR_BWD 2

volatile uint8_t angle;
volatile uint8_t direction=DIR_FWD;

#define OUTPUT PORTC
#define OUTPUT_PIN PC0

//наименьшее, наибольшее значения длительности в микросекундах
#define MIN_PERIOD 600
#define MAX_PERIOD 2500
#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define TICK_DURATION 8

//значения для управления:
#define ANGLE_MIN 10
#define ANGLE_MAX 170
#define GAS_MIN -120
#define GAS_MAX 120

uint16_t scale(uint8_t value)
{
    if (value < MIN_ANGLE) value = MIN_ANGLE;
    if ( value > MAX_ANGLE) value =  MAX_ANGLE;
    double result;
    result = (value - MIN_ANGLE)/(double)(MAX_ANGLE-MIN_ANGLE);
    result *= (MAX_PERIOD-MIN_PERIOD);
    result += MIN_PERIOD;
    result /= TICK_DURATION;
    return (uint16_t)result;
}

void setWidth(uint16_t pulseWidth)
{
    double result = pulseWidth / (double)TICK_DURATION;
    OCR1A = (uint16_t)result;
}


void setAngle(uint8_t value)
{
    OCR1A = scale(value);
}

void setTop(uint16_t value)
{
    OCR1B = value;
}

void setWidthTimer0(uint8_t width)
{
    OCR0 = width;
}

//-------------------------------------------
//TIMER1 Interruptions
//прерывание по совпадению с OCR1A
ISR(TIMER1_COMPA_vect)
{
    OUTPUT &= ~(1 << PC0);

}

ISR(TIMER1_COMPB_vect)
{
    OUTPUT |= (1 << PC0);

    //атомарный доступ к TCNT1
    unsigned int sreg;
    sreg = SREG;
    cli();
    TCNT1 = 0;
    SREG = sreg;
}

ISR(TIMER1_OVF_vect)
{
    //OUTPUT |= (1 << OUTPUT_PIN);
}
//-------------------------------------------

//-------------------------------------------
//TIMER0 Interruptions
ISR(TIMER0_OVF_vect)
{
    switch(direction)
    {
        case DIR_FWD:
            OUTPUT |= ((1 << PC2) | (0 << PC3));
            break;
        case DIR_BWD:
            OUTPUT |= ((0 << PC2) | (1 << PC3));
            break;
    }
}

ISR(TIMER0_COMP_vect)
{
    OUTPUT &= ~((1 << PC2) | (1 << PC3));
}

//-------------------------------------------

void initTimers()
{

    //TIMER1:

    TCNT1 = 0;
    TIMSK = (1 << OCIE1A) | (1 << OCIE1B);
    //прерывания по совпадению
    TCCR1A = (0 << COM1A1) | (0 << COM1A0);
    //normal mode, будет выдавать значение в порт вручную в прерываниях
    TCCR1B = (1<< CS11) | (1 << CS10);
    //prescaler 64
    //---------


    //---------
    //TIMER0
    TCNT0 = 0;
    TIMSK |= (1 << OCIE0) | (1 << TOIE0);
    TCCR0 = (1 << CS02) | (0 << CS01) | (1 << CS00);
}

int main(void)
{
    serialInit(SPEED9600);
    mirf_init(); //инициализация nrf24

    DDRC = 0xff;

    angle = 90;
    setAngle(angle);
    setWidthTimer0(30);

    //сброс TCNT1 на 20 мс
    setTop(2500); //для сервы
    // setTop(2500);  // для ШИМ

    _delay_ms(50); // задержка для радиомодуля

    mirf_config(); //конфигурация радиомодуля

    initTimers();
    sei();

 
    while(1)
    {
        if(serialAvailable()== 6)
        {
            uint8_t angle;
            int16_t gas;
            uint16_t checksum;
            uint8_t ending;
            angle = serialReadChar();
            serialRead((byte*) &gas, 2);
            serialRead((byte*) &checksum, 2);
            ending = serialReadChar();

            serialClearBuffer();

            if(ending != 0x4f)
            {
                serialWrite("hmm, length is not 6, you are mistaken!\n");
                continue;
            }
            if( ((uint16_t)gas ^ (uint16_t)angle) != checksum)
            {
                serialWrite("checksum incorrent :(\n");
                continue;
            }

 //           serialWrite("Angle: ");
 //           serialPrintIntLn(angle);
 //           serialWrite("Gas: ");
 //           serialPrintIntLn(gas);

            //установка действующих значений
            setAngle(angle); //угол поворота колёс

            if(gas < 0)//направление вращения колес
                direction = DIR_BWD;
            else
                direction = DIR_FWD;

            setWidthTimer0(abs(gas)); //частота вращения колёс

        }
    }

/*
    while(1)
    {
        uint8_t buf[10];
        while(!mirf_data_ready()) ; //ждем пока не будут готовы данные

        mirf_get_data(buf);

        uint8_t angle;
        int16_t gas;
        uint16_t checksum;
        uint8_t ending;

        angle = buf[0];
        memcpy((void*)&gas, buf+1,2);
        memcpy((void*)&checksum,buf+3,2);
        ending = buf[5];

        if(ending != 0x4f)
        {
            continue;
        }
        if( ((uint16_t)gas ^ (uint16_t)angle) != checksum)
        {
            continue;
        }

        //проверки входящих параметров, мало ли..
        if(gas < GAS_MIN) gas = GAS_MIN;
        if(gas > GAS_MAX) gas = GAS_MAX;

        if(angle < ANGLE_MIN) angle = ANGLE_MIN;
        if(angle > ANGLE_MAX) angle = ANGLE_MAX;

        //установка действующих значений:
        setAngle(angle); //угол поворота колёс

        if(gas < 0)//направление вращения колес
            direction = DIR_BWD;
        else
            direction = DIR_FWD;

        setWidthTimer0(abs(gas)); //частота вращения колёс

    }
*/
    serialEnd();
    return 0;
}
