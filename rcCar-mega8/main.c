/*
 */

#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "mirf.h"

//вперёд
#define DIR_FWD 1
//назад
#define DIR_BWD 2

volatile uint8_t angle; //текущий угол поворота колес
volatile int16_t gas;
volatile uint8_t direction=DIR_FWD; //направление вращения двигателя
volatile uint8_t motorActive=0;

//назначение портов для управления
#define OUTPUT PORTC
#define SERVO_PIN PC2
#define OUTPUT_A PC1
#define OUTPUT_B PC0

//наименьшее, наибольшее значения длительности в микросекундах
#define MIN_PERIOD 600
#define MAX_PERIOD 2500
#define MIN_ANGLE 0
#define MAX_ANGLE 180
//длительность одного тика таймера сервы в микросекундах
#define TICK_DURATION 8

//значения для управления:
#define ANGLE_MIN 5
#define ANGLE_MAX 175
#define GAS_MIN -120
#define GAS_MAX 120

//расчет длительности импульса (для сервопривода)[интерполяция]
//value - в градусах
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

//устанавливает величину длительности импульса для сервопривода
//функция для ручного управления, использовать только в отладочных целях
//value - в микросекундах
void setWidthTimer1(uint16_t value_ms)
{
    double result = value_ms / (double)TICK_DURATION;
    OCR1A = (uint16_t)result;
}

//устанавливает величину длительности импульса для сервопривода,
//value - угол поворота, value = [MIN_ANGLE..MAX_ANGLE]
void setAngle(uint8_t value_degree)
{
    OCR1A = scale(value_degree);
}


//устанавливает период импульсов для сервопривода
//при достижении TCNT этого значения, вызывается прерывание по совпадению с OCR1B
//value - в тиках
void setServoPeriod(uint16_t value_tick)
{
    OCR1B = value_tick;
}

//устанавливает величину длительности импульса для двигателя
//value = [0..GAS_MAX]
//направление вращения задается переменной direction
void setGas(uint8_t value)
{
    OCR2 = value;
}

//-------------------------------------------
//TIMER1 Interruptions
//прерывания для управления сервоприводом
//прерывание по совпадению с OCR1A
ISR(TIMER1_COMPA_vect)
{
    OUTPUT &= ~(1 << SERVO_PIN); //SERVO_PIN = 0

}

//прерывание по совпадению с OCR1B
ISR(TIMER1_COMPB_vect)
{
    OUTPUT |= (1 << SERVO_PIN); //SERVO_PIN = 1

    //атомарный доступ к TCNT1
    unsigned int sreg;
    sreg = SREG;
    cli();
    TCNT1 = 0;
    SREG = sreg;
}

//-------------------------------------------
//Прерывания для управления двигателем
//TIMER2 Interruptions
//переполнение регистра, установка начальных значений в OUTPUT_A и OUPUT_B
ISR(TIMER2_OVF_vect)
{
    switch(direction)
    {
        case DIR_FWD:
            OUTPUT |= ((1 << OUTPUT_A) | (0 << OUTPUT_B));
            break;
        case DIR_BWD:
            OUTPUT |= ((0 << OUTPUT_A) | (1 << OUTPUT_B));
            break;
    }
}
//прерывание по совпадению с OCR2, сброс значений OUTPUT_A и OUTPUT_B
ISR(TIMER2_COMP_vect)
{
    OUTPUT &= ~((1 << OUTPUT_A) | (1 << OUTPUT_B));
}

//----------------------------------------
//Инициализация таймеров для управления сервоприводом
//
void initTimerServo()
{
    //TIMER1:
    TCNT1 = 0;
    TIMSK = (1 << OCIE1A) | (1 << OCIE1B);
    //прерывания по совпадению
    TCCR1A = (0 << COM1A1) | (0 << COM1A0);
    //normal mode, будет выдавать значение в порт вручную в прерываниях
    TCCR1B = (1<< CS11) | (1 << CS10);
    //prescaler 64
}

//Инициализация таймеров для управления сервоприводом
void initTimerMotor()
{
    //TIMER2:
    TCNT2 = 0;
    TIMSK |= (1 << OCIE2) | (1 << TOIE2);
    TCCR2 = (1 << CS22) | (0 << CS21) | (1 << CS20);
}
//-------------------------------------------

void pauseTimerMotor()
{
    TCCR2 = 0;
    motorActive = 0;
}

void resumeTimerMotor()
{
    TCCR2 = (1 << CS22) | (0 << CS21) | (1 << CS20);
    motorActive = 1;
}

int main(void)
{
    serialInit(SPEED9600); //инициализация serial-порта
    mirf_init(); //инициализация nrf24

    DDRC = 0xff;

    angle = 90;
    gas = 0;

    setAngle(angle);
    setServoPeriod(2500);//установка периода импульсов для сервопривода

    setGas(gas);

    _delay_ms(50); // задержка для радиомодуля

    mirf_config(); //конфигурация радиомодуля

    initTimerServo(); //инициализация таймеров
    initTimerMotor(); //
    pauseTimerMotor(); //приостановим таймер для двигателя

    sei();
/*
    //отладочный код
    //получает задающие значения по uart
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

            setWidthTimer2(abs(gas)); //частота вращения колёс

        }
    }
*/

    uint8_t tmp_angle;
    int16_t tmp_gas;
    //получаем задающие значения по радиоканалу
    while(1)
    {
        uint8_t buf[10];
        while(!mirf_data_ready()) ; //ждем пока не будут готовы данные

        mirf_get_data(buf);

        uint16_t checksum;
        uint8_t ending;

        tmp_angle = buf[0];
        memcpy((void*)&tmp_gas, buf+1,2);
        memcpy((void*)&checksum,buf+3,2);
        ending = buf[5];

        if(ending != 0x4f)
        {
            continue;
        }
        if( ((uint16_t)tmp_gas ^ (uint16_t)tmp_angle) != checksum)
        {
            continue;
        }


        //установка действующих значений:

        if(angle != tmp_angle)
        {
            angle = tmp_angle;

            if(angle < ANGLE_MIN) angle = ANGLE_MIN;
            if(angle > ANGLE_MAX) angle = ANGLE_MAX;

            setAngle(angle); //угол поворота колёс
        }

        if(tmp_gas == 0)
        {
            if(motorActive)
                pauseTimerMotor();
        }
        else
        {
            if(!motorActive)
                resumeTimerMotor();

            if(gas != tmp_gas)
            {
            gas = tmp_gas;
                        //проверки входящих параметров, мало ли..
            if(gas < GAS_MIN) gas = GAS_MIN;
            if(gas > GAS_MAX) gas = GAS_MAX;

            if(gas < 0)//направление вращения колес
                direction = DIR_BWD;
            else
                direction = DIR_FWD;

            setGas(abs(gas)); //частота вращения колёс
            }
        }



    }

    serialEnd();
    return 0;
}
