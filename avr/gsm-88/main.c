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
#define GSMBUF_LENGTH 64
#define GPSBUF_LENGTH 80
#define TEL_LENGTH 15


data position;
volatile uint8_t msg_counter=0;
volatile uint8_t gsm_ok=0;
volatile uint16_t cycles=0;
const char myTelephone[] PROGMEM = "+79512962477";
static char gpsBuf[GPSBUF_LENGTH];
static uint8_t gpsBufCnt = 0;

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
    SUART_PutStrFl((char*)PSTR("AT+CMGS=\""));
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
    LcdClear();

    LcdGotoXYFont(0,0);
    if(position.valid)
        LcdFStr(FONT_1X, (const unsigned char*)PSTR("GPS: OK"));
    else
        LcdFStr(FONT_1X, (const unsigned char*)PSTR("GPS: NOTVALID"));


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

    LcdUpdate();
}

void inline ProcessGPS()
{
    if(serialAvailable())
    {
        gpsBufCnt = serialReadUntil(gpsBuf, GPSBUF_LENGTH, '\r');
        gpsBuf[gpsBufCnt] = '\0';

        parseGPRMC(gpsBuf, &position);
        serialClearBuffer();

    }
}

int main()
{

    SUART_Init();
    serialInit(SPEED9600);
    LcdInit();

    sei();

    LcdClear();
    LcdGotoXYFont(0,0);
    LcdChr(FONT_1X, '*');
    LcdUpdate();

    //try to Power-on GSM
    if(!(gsm_ok = GSM_Test2()))
    {
        GSM_PowerOn();
        _delay_ms(6000);
    }
    gsm_ok = GSM_Test2();
    GSM_Init2();

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
                            SendPosition();
                            SUART_FlushInBuf();
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
                                SendPosition();
                                SUART_FlushInBuf();
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
        //--------------- GSM  -------------------------------------
        ProcessDisplay();
        cycles++;
    }


    return 0;
}
