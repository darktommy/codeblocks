// В настройках проекта обязательно правильно укажите свою тактовую частоту
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "compilers_3.h"
#include "serial.h"
#include <string.h>
#include <stdlib.h>
#include "softUart.h"
#include "gsm.h"
#include "gps.h"

#define POSITION_TOKEN "Pos"
#define SPEED_TOKEN "Speed"
#define SPYMODEON_TOKEN "Spy"
#define SLEEP_TOKEN "Off"

#define GSMBUF_LENGTH 80
#define GPSBUF_LENGTH 80
#define TEL_LENGTH 15

#define SPY_MODE 1 //gps не выключаем
#define UPDATE_MODE 2 //gps не отключаем до первых актуальных данных, затем выкл. gps
#define IDLE_MODE 3 //gps выкл

#define GPS_OFF PORTD &= ~(1 << PD2)
#define GPS_ON  PORTD |= (1 << PD2)

volatile static uint8_t currentMode = 0; //режим работы устройства
volatile uint16_t timer1_ovfs = 0;
volatile uint16_t timer2_ovfs = 0;
volatile static uint8_t needToSendPos = 0;
volatile static uint8_t needToSendSpeed = 0;
volatile static uint8_t gsm_ok=0;

static data position;
const char myTelephone[] PROGMEM = "+79512962477";
static char gpsBuf[GPSBUF_LENGTH];
volatile static uint8_t gpsBufCnt = 0;
static char gsmBuf[GPSBUF_LENGTH];
volatile static uint8_t gsmBufCnt=0;


//переводит систему в заданный режим
void SetMode(uint8_t mode)
{
    if(mode != currentMode)
    {
        cli();
        switch (mode)
        {
        case IDLE_MODE:
            GPS_OFF;
            TCNT1 = 0;
            TIMSK1 = (1 << TOIE1);
            TCCR1A = 0;
            TCCR1B = (1 << CS12); //prescaler = 256
            break;
        case UPDATE_MODE:
        case SPY_MODE:
            GPS_ON;
            TCCR1B = 0;
            TCNT1 = 0;
            break;
        }
        currentMode = mode;
        sei();
    }

}

//таймер для отслеживания момента вкл/выкл GPS
ISR(TIMER1_OVF_vect)
{
    timer1_ovfs++;
    if(timer1_ovfs >= 570) //2 мин = 570, 2 мин = 57
    {
        SetMode(UPDATE_MODE);
        timer1_ovfs = 0;
    }
}

// сделать URL на googleMaps из координат и отправить
void SendURL()
{
    char outstr[55];
    memset(outstr, 0, 55);
    strcpy_P(outstr, (const char*)PSTR("http://google.ru/#q="));
    if(position.NS == 'S')
        strcat(outstr, "-");
    strncat(outstr, position.strLatitude, 2);
    strcat_P(outstr, (const char*)PSTR("+"));
    strcat(outstr, position.strLatitude+2);
    strcat_P(outstr, (const char*)PSTR("%2C"));
    if(position.WE == 'W')
        strcat(outstr, "-");
    strncat(outstr, position.strLongtitude+1, 2);
    strcat_P(outstr, (const char*)PSTR("+"));
    strcat(outstr, position.strLongtitude+3);

    SUART_PutStrFl((char*)PSTR(" AT+CMGS=\"")); //1й символ не воспринимается gsm модулем, т.к. в энергосберег.режиме
    SUART_PutStrFl((char*)myTelephone);
    SUART_PutChar('"');
    SUART_PutChar('\r');
    _delay_ms(100);
    SUART_PutStr(outstr);
    SUART_PutChar(26);//Ctrl-Z
}

// отправить сообщение с координатами
void SendPosition()
{
    SUART_PutStrFl((char*)PSTR(" AT+CMGS=\"")); //1й символ не воспринимается gsm модулем, т.к. в энергосберег.режиме
    SUART_PutStrFl((char*)myTelephone);
    SUART_PutChar('"');
    SUART_PutChar('\r');
    _delay_ms(100);

    if(position.NS == 'S')
        SUART_PutChar('-');
    SUART_PutStr(position.strLatitude);
    SUART_PutChar(';');

    if(position.WE == 'W')
        SUART_PutChar('-');
    SUART_PutStr(position.strLongtitude);

    SUART_PutChar(26);//Ctrl-Z

    SUART_FlushInBuf();
}

void SendSpeed()
{
    SUART_PutStrFl((char*)PSTR(" AT+CMGS=\"")); //1й символ не воспринимается gsm модулем, т.к. в энергосберег.режиме
    SUART_PutStrFl((char*)myTelephone);
    SUART_PutChar('"');
    SUART_PutChar('\r');
    _delay_ms(100);

    SUART_PutStr(position.strSpeed);

    SUART_PutChar(26);//Ctrl-Z

    SUART_FlushInBuf();
}

int main()
{
    DDRD |= (1 << PD2); //управление питанием GPS

    SUART_Init();
    serialInit(SPEED9600);
    sei();

    SetMode(UPDATE_MODE);

    uint8_t i;
    for (i=0;i<3;i++)
    {
        gsm_ok = GSM_Test();
        if(gsm_ok) break;
        _delay_ms(1000);
    }

    if(!gsm_ok)
    {
        GSM_PowerOn();
        _delay_ms(7000);
    }

    needToSendPos = 0;
    needToSendSpeed = 0;

    GSM_Init();
    SUART_FlushInBuf();

    char volatile c;
    char *p1,*p2;
    char tel[TEL_LENGTH];

    while(1)
    {
        /* GPS */
        if(currentMode != IDLE_MODE)
        {
            if(serialAvailable())
            {
                gpsBufCnt = serialReadUntil(gpsBuf, GPSBUF_LENGTH, '\r');
                gpsBuf[gpsBufCnt] = '\0';

                if(parseGPRMC(gpsBuf, &position))
                {
                    if (currentMode == UPDATE_MODE)
                        SetMode(IDLE_MODE);
                    /* лучше бы отправлять сообщнения отсюда, но у меня почему-то не работает,
                       фигня какаято. */
                    /*
                    if(needToSendPos)
                    {
                        needToSendPos = 0;
                        SendPosition();
                    }
                    if(needToSendSpeed)
                    {
                        needToSendSpeed = 0;
                        SendSpeed();
                    }*/

                }
                serialClearBuffer();
            }
        }

        /* Отправка необходимых сообщений */
        if(needToSendPos)
        {
            if(position.valid)
            {
                needToSendPos = 0;
                SendPosition();
            }
        }
        if(needToSendSpeed)
        {
            if(position.valid)
            {
                needToSendSpeed = 0;
                SendSpeed();
            }
        }

        /* GSM */
        while(SUART_Kbhit())
        {
            c = SUART_GetChar();
            if (c == '\n')
            {
                gsmBuf[gsmBufCnt] = '\0';
                gsmBufCnt = 0;

                if(strstr(gsmBuf, "+CLIP"))
                {
                    p1 = strchr(gsmBuf, '"');
                    p2 = strchr(p1+1, '"');

                    if(p1 && p2)
                    {
                        strlcpy(tel, p1+1, p2-p1);
                        // проверка номера телефона.
                        if(strcmp_P( tel, myTelephone) == 0)//пришло от меня
                        {
                            needToSendPos = 1;
                            if (currentMode == IDLE_MODE)
                            {
                                SetMode(UPDATE_MODE);
                            }

                            SUART_PutStrFl((char*)PSTR("ATH\r"));
                            _delay_ms(2000);
                            SUART_FlushInBuf();
                            break;
                        }

                    }
                }
                if(strstr(gsmBuf, "+CMT"))
                {
                    p1 = strchr(gsmBuf, '"');
                    p2 = strchr(p1+1, '"');

                    if(p1 && p2)
                    {
                        strlcpy(tel, p1+1, p2-p1);
                        if(strcmp_P( tel, myTelephone) == 0)//пришло от меня
                        {
                            gsmBufCnt=0;
                            while(1)
                            {
                                c = SUART_GetChar();
                                gsmBuf[gsmBufCnt] = c;
                                gsmBufCnt++;
                                if (c == '\n')
                                {
                                    gsmBuf[gsmBufCnt] = '\0';
                                    break;
                                }
                            }
                            if (strstr(gsmBuf, SPYMODEON_TOKEN))
                            {
                                SetMode(SPY_MODE);
                                SUART_FlushInBuf();
                                break;
                            }
                            else if (strstr(gsmBuf, SLEEP_TOKEN))
                            {
                                SetMode(IDLE_MODE);
                                SUART_FlushInBuf();
                                break;
                            }
                            else if (strstr(gsmBuf, SPEED_TOKEN))
                            {
                                needToSendSpeed = 1;
                                if (currentMode == IDLE_MODE)
                                    SetMode(UPDATE_MODE);
                                _delay_ms(2000);
                                SUART_FlushInBuf();
                                break;
                            }
                        }
                    }
                }

            }
            else
            {
                gsmBuf[gsmBufCnt] = c;
                gsmBufCnt++;
            }
        }
    }


    return 0;
}

