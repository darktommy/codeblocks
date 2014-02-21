// В настройках проекта обязательно правильно укажите свою тактовую частоту
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "serial.h"
#include "nmea.h"
#include "n3310.h"
//#include "picture.h"

typedef struct _data
{
    double latitude;
    double longtitude;
    uint8_t valid;
}data;

data position;

char* next_field(char* buf)
{
    while(*buf++ != ',')
        ;

    return buf;
}

uint8_t parse(char* buf, data* d)
{
    buf = strstr(buf, "GPGLL");
    if( buf == 0 ) return -1;

    buf = next_field(buf); //first field
    if(*buf != ',')
        (*d).latitude = atof(buf);

    buf = next_field(buf);
    if(*buf == 'S')
        (*d).latitude = -(*d).latitude;

    buf = next_field(buf);
    if(*buf != ',')
        (*d).longtitude = atof(buf);

    buf = next_field(buf);
    if(*buf == 'W')
        (*d).longtitude = -(*d).longtitude;

    buf = next_field(buf); //skip UTC time
    buf = next_field(buf);

    if(*buf == 'A')
        (*d).valid = 1;
    else
        (*d).valid = 0;

    //TODO: checksum

    return 0;

}

int main()
{

    serialInit(SPEED9600);
    sei();

    LcdInit();

    char buf[256];
    char strbuf[50];
    uint8_t buf_len;

    memset(buf,0,255);
    memset(strbuf,0,50);

    while(1)
    {
        if(serialAvailable()>0)
        {
            /*memset(buf, 0, 256);

            buf_len = serialReadUntil(buf, 255,'\r');
            serialClearBuffer();

            LcdClear();
            LcdGotoXYFont(0,2);
            itoa(buf_len,strbuf, 10);
            LcdStr(FONT_1X, strbuf);

            if(nmea_parser(buf) != -1)
            {
                if(nmea_rmc_data.valid)
                {
                    LcdGotoXYFont(0,0);
                    dtostrf(nmea_rmc_data.latitude,2,2,strbuf);
                    LcdStr(FONT_1X, (byte*)strbuf);
                    memset(strbuf,0,50);
                    LcdGotoXYFont(0,1);
                    dtostrf(nmea_rmc_data.longitude,2,2,strbuf);
                    LcdStr(FONT_1X, (byte*)strbuf);
                }
                else
                {
                    LcdGotoXYFont(0,0);
                    LcdFStr(FONT_1X,(unsigned char*)PSTR("Invalid Data"));
                }
            }

            LcdUpdate();*/

            buf_len = serialReadUntil(buf, 255, '\r');
            buf[buf_len] = '\0';
            serialClearBuffer();
            if(parse(buf, &position) == 0)
            {
                LcdClear();
                dtostrf(position.latitude, 2,2,strbuf);
                LcdGotoXYFont(0,0);
                LcdStr(FONT_1X, (byte*)strbuf);

                dtostrf(position.longtitude, 2,2,strbuf);
                LcdGotoXYFont(0,1);
                LcdStr(FONT_1X, (byte*)strbuf);

                if(position.valid != 1)
                {
                    LcdGotoXYFont(0,5);
                    LcdStr(FONT_1X,(byte*)"not ready");
                }
                LcdUpdate();

            }

        }
    }

    return 0;
}
