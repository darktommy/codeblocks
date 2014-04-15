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
#define TRACKMODEON_TOKEN "Track"
#define SPYMODEON_TOKEN "Spy"
#define SLEEP_TOKEN "Off"

#define GSMBUF_LENGTH 80
#define GPSBUF_LENGTH 90
#define TEL_LENGTH 15

#define SPY_MODE 0 //gps не выключаем
#define TRACK_MODE 1 //gps не выключаем + каждые 2 минуты отправляем координаты
#define UPDATE_MODE 2 //gps не отключаем до первых актуальных данных, затем выкл. gps
#define IDLE_MODE 3 //gps выкл

#define GPS_OFF PORTD &= ~(1 << PD2)
#define GPS_ON  PORTD |= (1 << PD2)

uint8_t volatile currentMode = 4; //режим работы устройства

uint16_t volatile timer1_ovfs = 0;
uint16_t volatile timer2_ovfs = 0;
uint8_t volatile needToSendSms = 0;

data position;
uint8_t volatile gsm_ok=0;
const char myTelephone[] PROGMEM = "+79512962477";

static char gpsBuf[GPSBUF_LENGTH];
static volatile uint8_t gpsBufCnt = 0;
static char gsmBuf[GPSBUF_LENGTH];
static volatile uint8_t gsmBufCnt=0;


void SetTimer1Mode(uint8_t mode) //1 - on, 0 - off
{
    switch (mode)
    {
    case 0:
        TCCR1B = 0;
        TCNT1 = 0;
        break;
    case 1:
        TCNT1 = 0;
        TIMSK1 = (1 << TOIE1);
        TCCR1A = 0;
        TCCR1B = (1 << CS12); //prescaler = 256
        break;
    }
}

void SetTimer2Mode(uint8_t mode) //1 - on, 0 - off
{
    switch (mode)
    {
    case 0:
        TCCR2B = 0;
        TCNT2 = 0;
        break;
    case 1:
        TCNT2 = 0;
        TIMSK2 = (1 << TOIE2);
        TCCR2A = 0;
        TCCR2B = (7 << CS20); //prescaler = 1024
        break;
    }
}

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
            SetTimer1Mode(1);
            SetTimer2Mode(0);
            break;
        case UPDATE_MODE:
            GPS_ON;
            SetTimer1Mode(0);
            SetTimer2Mode(0);
            break;
        case SPY_MODE:
            GPS_ON;
            SetTimer1Mode(0);
            SetTimer2Mode(0);
            break;
        case TRACK_MODE:
            GPS_ON;
            SetTimer1Mode(0);
            SetTimer2Mode(1);
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
    if(timer1_ovfs >= 57) //2 часа = 3444, 2 мин = 57
    {
        SetMode(UPDATE_MODE);
        timer1_ovfs = 0;
    }
}

//таймер для отслеживания момента отправки сообщений в режиме TRACK
ISR(TIMER2_OVF_vect)
{
    timer2_ovfs++;
    if(timer2_ovfs >= 3662)
    {
        needToSendSms = 1;
        timer2_ovfs = 0;
    }
}



// сделать URL на googleMaps из координат и отправить
void SendURL()
{
    char outstr[50];
    memset(outstr, 0, 50);
    strcpy_P(outstr, (const char*)PSTR("http://google.ru/#q="));
    strncat(outstr, position.strLatitude, 2);
    strcat_P(outstr, (const char*)PSTR("+"));
    strcat(outstr, position.strLatitude+2);
    strcat_P(outstr, (const char*)PSTR("%2C"));
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
    SUART_PutStr(position.strLatitude);
    SUART_PutChar(';');
    SUART_PutStr(position.strLongtitude);
    SUART_PutChar(26);//Ctrl-Z

    SUART_FlushInBuf();
}

void SendSMS()
{
    SUART_PutStrFl((char*)PSTR(" AT+CMGS=\"")); //1й символ не воспринимается gsm модулем, т.к. в энергосберег.режиме
    SUART_PutStrFl((char*)myTelephone);
    SUART_PutChar('"');
    SUART_PutChar('\r');
    _delay_ms(100);
    SUART_PutStr("Hello Max!");
    SUART_PutChar(26);//Ctrl-Z
}

void ProcessGPS()
{
    if(currentMode != IDLE_MODE)
    {
        if(serialAvailable())
        {
            gpsBufCnt = serialReadUntil(gpsBuf, GPSBUF_LENGTH, '\r');
            gpsBuf[gpsBufCnt] = '\0';

            if(parseGPRMC(gpsBuf, &position))
            {
                if (needToSendSms)
                {
                    SendPosition();
                    needToSendSms = 0;
                }
//                if (currentMode == UPDATE_MODE)
//                    SetMode(IDLE_MODE);
            }
            serialClearBuffer();
        }
    }
}


int main()
{
    DDRD |= (1 << PD2); //управление питанием GPS

    SUART_Init();
    serialInit(SPEED9600);
    sei();

    SetMode(SPY_MODE);

    uint8_t i;
    for (i=0;i<2;i++)
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

    GSM_Init();
    SUART_FlushInBuf();

    char volatile c;
    char *p1,*p2;
    char tel[TEL_LENGTH];

    while(1)
    {
        ProcessGPS();

        //--------------- GSM  -----------------------------------
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
//                            if (currentMode == IDLE_MODE)
//                                SetMode(UPDATE_MODE);
                            SUART_PutStrFl((char*)PSTR("ATH\r"));
                            _delay_ms(1500);
                            needToSendSms = 1;
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
                            if(strstr(gsmBuf, POSITION_TOKEN))
                            {
//                                if (currentMode == IDLE_MODE)
//                                    SetMode(UPDATE_MODE);
                                needToSendSms = 1;
                                SUART_FlushInBuf();
                                break;
                            }/*
                            else if (strstr(gsmBuf, SPYMODEON_TOKEN))
                            {
                                SetMode(SPY_MODE);
                                SUART_FlushInBuf();
                                break;
                            }
                            else if (strstr(gsmBuf, TRACKMODEON_TOKEN))
                            {
                                SetMode(TRACK_MODE);
                                SUART_FlushInBuf();
                                break;
                            }
                            else if (strstr(gsmBuf, SLEEP_TOKEN))
                            {
                                SetMode(IDLE_MODE);
                                SUART_FlushInBuf();
                                break;
                            }*/

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
        //--------------- GSM end -------------------------------------

    }


    return 0;
}

