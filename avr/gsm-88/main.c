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
#define TEL_LENGTH 15


data position;
volatile uint8_t msg_counter=0;
volatile uint8_t gsm_ok=0;
volatile uint16_t cycles=0;
volatile uint8_t available=0;
const char myTelephone[] PROGMEM = "+79512962477";

// отправить сообщение с координатами
void SendPosition()
{
    serialWriteF((const unsigned char*)PSTR("AT+CMGS=\""));
    serialWriteF((const unsigned char*)myTelephone);
    serialWriteChar('"');
    serialWriteChar('\r');
    _delay_ms(100);
    if(position.valid)
    {
        serialWrite(position.strLatitude);
        serialWriteChar(',');
        serialWrite(position.strLongtitude);
    }
    else
        serialWriteF((const unsigned char*)PSTR("GPS not ready :("));

    serialWriteChar(26);//Ctrl-Z

    msg_counter++;
}


void ProcessGSM()
{
    char buf[GSMBUF_LENGTH];
    uint8_t buflen=0;
    char *p1,*p2;
    char tel[TEL_LENGTH];

    available = serialAvailable();
    if(available < 4)
        return;

    if(serialSearchChr('\r'))
    {
        buflen = serialReadUntil(buf, GSMBUF_LENGTH, '\r');
        buf[buflen] = '\0';
    }
    else
        return;

    if((p1 = strstr(buf, "+CMT")))
    {
        //TODO: понять, от кого сообщение
        p1 = strchr(buf, '"');
        p2 = strchr(p1+1, '"');
        if(p1 && p2)
        {
            strlcpy(tel, p1+1, p2-p1);
            if(strcmp_P( tel, myTelephone) == 0)//пришло от меня
            {
                //buflen = serialReadUntil(buf, GSMBUF_LENGTH, '\r');
                buflen = serialReadAll(buf);
                buf[buflen] = '\0';

                if(strstr(buf, POSITION_TOKEN))
                {
                    SendPosition();
                }
            }
        }
    }
    else
    {
        if(strstr(buf, "RING"))
        {
            // узнать номер звонящего
            buflen = serialReadAll(buf);
            buf[buflen] = '\0';
            serialClearBuffer();

            // вешаем трубку
            serialWriteF((const unsigned char*)PSTR("ATH\r"));

            _delay_ms(500);
            p1 = strchr(buf, '"');
            p2 = strchr(p1+1, '"');
            if(p1 && p2)
            {
                strlcpy(tel, p1+1, p2-p1);
                // проверка номера телефона.
                if(strcmp_P( tel, myTelephone) == 0)//пришло от меня
                {
                    SendPosition();
                }

            }

        }
    }
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
        LcdFStr(FONT_1X, (const unsigned char*)PSTR("GSM: UNAVAILABLE"));

    if(position.valid)
    {
        LcdGotoXYFont(0,2);
        LcdStr(FONT_1X, (byte*)position.strLatitude);

        LcdGotoXYFont(0,3);
        LcdStr(FONT_1X, (byte*)position.strLongtitude);
    }


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
    if(!(gsm_ok = GSM_Test()))
    {
        GSM_PowerOn();
        _delay_ms(6000);
    }

    gsm_ok = GSM_Test();

    GSM_Init();

    char buf[64];
    uint8_t bufcnt=0;
    char c;

    while(1)
    {
        ProcessGSM();

        /************** принимаем данные с GPS ******************/
        while(SUART_Kbhit())
        {
            c = SUART_GetChar();
            if (c == '\n' || c == '\r')
            {
                buf[bufcnt] = '\0';
                bufcnt = 0;

                if(parseGPRMC(buf, &position) == 0)
                    SUART_FlushInBuf();
            }
            else
            {
                buf[bufcnt] = c;
                bufcnt++;
            }

        }
        /*********************************************************/
        ProcessDisplay();
        cycles++;
    }



    return 0;
}
