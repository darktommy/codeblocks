// В настройках проекта обязательно правильно укажите свою тактовую частоту
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "compilers_3.h"
#include "serial.h"
#include <string.h>
#include <stdlib.h>
#include "softUart.h"
#include "n3310.h"
#include "gsm.h"
#include "gps.h"

#define POSITION_TOKEN "Pos"
#define SPEED_TOKEN "Speed"
#define TRACKMODEON_TOKEN "Track"
#define TRACKMODEOFF_TOKEN "TrackOff"
#define SPYMODEON_TOKEN "Spy"
#define SPYMODEOFF_TOKEN "SpyOff"
#define SLEEP_TOKEN "Off"

#define GSMBUF_LENGTH 64
#define GPSBUF_LENGTH 80
#define TEL_LENGTH 15

#define SPY_MODE 0 //gps не выключаем
#define TRACK_MODE 1 //gps не выключаем + каждые 2 минуты отправляем координаты
#define UPDATE_MODE 2 //gps не отключаем до первых актуальных данных, затем выкл. gps
#define IDLE_MODE 3 //gps выкл

uint8_t currentMode = IDLE_MODE; //режим работы устройства

uint16_t timer1_ovfs = 0;
uint16_t timer2_ovfs = 0;
uint8_t needToSendSms = 0;

data position;
volatile uint8_t msg_counter=0;
volatile uint8_t gsm_ok=0;
volatile uint16_t cycles=0;
const char myTelephone[] PROGMEM = "+79512962477";
static char gpsBuf[GPSBUF_LENGTH];
static uint8_t gpsBufCnt = 0;

void SetMode(uint8_t mode);

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

void inline GPS_OFF()
{
    //TODO выключаем GPS
    PORTB &= ~(1 << PB1);
    PORTC &= ~(1 << PC3);
}

void inline GPS_ON()
{
    //TODO включаем GPS
    PORTB |= (1 << PB1);
    PORTC |= (1 << PC3);
}

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


void SetMode(uint8_t mode)
{
    if(mode != currentMode)
    {
        cli();
        switch (mode)
        {
        case IDLE_MODE:
            GPS_OFF();
            SetTimer1Mode(1);
            SetTimer2Mode(0);
            break;
        case UPDATE_MODE:
            GPS_ON();
            SetTimer1Mode(0);
            SetTimer2Mode(0);
            break;
        case SPY_MODE:
            GPS_ON();
            SetTimer1Mode(0);
            SetTimer2Mode(0);
            break;
        case TRACK_MODE:
            GPS_ON();
            SetTimer1Mode(0);
            SetTimer2Mode(1);
            break;
        }
        currentMode = mode;
        sei();
    }

}


// сделать URL из координат
void MakeURL(char *outstr)
{
    strcpy_P(outstr, (const char*)PSTR("http://google.ru/#q="));
    strncat(outstr, position.strLatitude, 2);
    strcat_P(outstr, (const char*)PSTR("+"));
    strcat(outstr, position.strLatitude+2);
    strcat_P(outstr, (const char*)PSTR("%2C"));
    strncat(outstr, position.strLongtitude+1, 2);
    strcat_P(outstr, (const char*)PSTR("+"));
    strcat(outstr, position.strLongtitude+3);
}

// отправить сообщение с координатами
void SendPosition()
{
    char url[60];
    SUART_PutStrFl((char*)PSTR("  AT+CMGS=\"")); //первый символ не читается, т.к. энергосберегающий режим
    SUART_PutStrFl((char*)myTelephone);
    SUART_PutChar('"');
    SUART_PutChar('\r');
    _delay_ms(100);
    if(position.valid)
    {
        MakeURL(url);
        SUART_PutStr(url);
    }
    else
        SUART_PutStrFl((char*)PSTR("GPS not ready :("));

    SUART_PutChar(26);//Ctrl-Z

    msg_counter++;
}


void ProcessDisplay()
{
    /*
    LcdClear();

    LcdGotoXYFont(0,0);
    if(currentMode != IDLE_MODE)
        LcdFStr(FONT_1X, (const unsigned char*)PSTR("GPS: ON"));
    else
        LcdFStr(FONT_1X, (const unsigned char*)PSTR("GPS: OFF"));


    LcdGotoXYFont(0,1);
    if(gsm_ok)
        LcdFStr(FONT_1X, (const unsigned char*)PSTR("GSM: OK"));
    else
        LcdFStr(FONT_1X, (const unsigned char*)PSTR("GSM: FAIL"));

    //if(position.valid)
    //{
        LcdGotoXYFont(0,2);
        LcdStr(FONT_1X, (byte*)position.strLatitude);

        LcdGotoXYFont(0,3);
        LcdStr(FONT_1X, (byte*)position.strLongtitude);
    //}


    char str[6];

    LcdGotoXYFont(0,4);
    LcdFStr(FONT_1X, (const unsigned char*)PSTR("MSGs: "));
    itoa(msg_counter, str, 10);
    LcdStr(FONT_1X, (byte*)str);

    LcdGotoXYFont(0,5);
    itoa(cycles, str, 10);
    LcdStr(FONT_1X, (byte*)str);

    LcdUpdate();*/

/*
    if(currentMode != IDLE_MODE)
        PORTB |= (1 << PB0);
    else
        PORTB &= ~(1 << PB0);
*/
    if(needToSendSms)
        PORTB |= (1 << PB0);
    else
        PORTB &= ~(1 << PB0);

}

void inline ProcessGPS()
{
    if(currentMode != IDLE_MODE)
    {
        if(serialAvailable())
        {
            gpsBufCnt = serialReadUntil(gpsBuf, GPSBUF_LENGTH, '\r');
            gpsBuf[gpsBufCnt] = '\0';

            if(parseGPRMC(gpsBuf, &position))
            {
            //TODO
                if (needToSendSms)
                {
                    SendPosition();
                    needToSendSms = 0;
                }
                if (currentMode == UPDATE_MODE)
                    SetMode(IDLE_MODE);
            }
            serialClearBuffer();

        }
    }
}



int main()
{

    SUART_Init();
    serialInit(SPEED9600);
   // LcdInit();

    DDRB = 0xff;
    PORTB = 0xff;

    sei();
/*
    LcdClear();
    LcdGotoXYFont(0,0);
    LcdChr(FONT_1X, '*');
    LcdUpdate();
*/
    //try to Power-on GSM
    if(!(gsm_ok = GSM_Test2()))
    {
        GSM_PowerOn();
        _delay_ms(6000);
    }
    gsm_ok = GSM_Test2();
    GSM_Init2();

    PORTB = 0x00;

    SetMode(SPY_MODE);

    char gsmBuf[GPSBUF_LENGTH];
    uint8_t gsmBufCnt=0;
    char c;
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

                    SUART_PutStrFl((char*)PSTR("ATH\r"));
                    _delay_ms(1500);

                    if(p1 && p2)
                    {
                        strlcpy(tel, p1+1, p2-p1);
                        // проверка номера телефона.
                        if(strcmp_P( tel, myTelephone) == 0)//пришло от меня
                        {
                            if (currentMode == IDLE_MODE)
                                SetMode(UPDATE_MODE);
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
                                if (currentMode == IDLE_MODE)
                                    SetMode(UPDATE_MODE);
                                needToSendSms = 1;
                                SUART_FlushInBuf();
                                break;
                            }
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
        //--------------- GSM end -------------------------------------

        ProcessDisplay();
        cycles++;

        if(cycles % 2 == 0)
        {
            PORTB |= (1 << PB3);
            _delay_ms(100);
            PORTB &= ~(1 << PB3);
        }
    }


    return 0;
}
