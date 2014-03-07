// В настройках проекта обязательно правильно укажите свою тактовую частоту
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "compilers_3.h"
//#include "serial.h"
#include <string.h>
#include <stdlib.h>
#include "softUart.h"
#include "n3310.h"


typedef struct _data
{
    double latitude;
    double longtitude;
    uint8_t valid;
}data;

data position;

unsigned char hex2bin( char c )
{
  if( ( c >= 'A') && ( c <= 'F') ) {
    return c - 'A';
  }
  else if( ( c >= 'a') && ( c <= 'f') ) {
    return c - 'a';
  }
  else {
    return c - '0';
  }
}


uint8_t check_crc( char *buf )
{
  unsigned char crc = 0;
  while( *buf != '*' ) {
     crc ^= *buf++;
  }

  return ( ( crc & 0x0F ) == hex2bin( *(buf+2) ) ) &&
         ( ( crc >> 4   ) == hex2bin( *(buf+1) ) );
}

uint8_t get_crc( char * buf)
{
    uint8_t crc = 0;
    while( *buf != '*' ) {
     crc ^= *buf++;
    }
    return crc;
}


char* next_field(char* buf)
{
    while(*buf++ != ',')
        ;

    return buf;
}



uint8_t parse(char* buf, data* d)
{
    /*
    buf = strstr(buf, "GPGLL");
    if( buf == 0 ) return -1;*/

    buf = strchr(buf, '$');
    if(buf == 0) return -1;

    buf++;

    if(strncmp(buf, "GPGLL", 5) != 0) return -1;

    if(!check_crc(buf))
        return -1;


    buf = next_field(buf);
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

    return 0;

}

int main()
{

    //serialInit(SPEED9600);
    SUART_Init();//9600 8n1
    sei();

    LcdInit();

    char buf[256];
    char strbuf[50];

    memset(buf,0,255);
    memset(strbuf,0,50);

    //для программного UART
    uint8_t bufcnt=0;
    char c;
    while(1)
    {

        if(SUART_Kbhit())
        {
            c = SUART_GetChar();
            if (c == '\n' || c == '\r')
            {
                buf[bufcnt] = '\0';
                bufcnt = 0;

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
            else
            {
                buf[bufcnt] = c;
                bufcnt++;
            }

        }
    }

    return 0;
}
