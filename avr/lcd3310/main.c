// В настройках проекта обязательно правильно укажите свою тактовую частоту
#define F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "n3310.h"
//#include "picture.h"

const unsigned char getHardwareInfo[] =
{
    0x1E,0x00,0x0C,0xD1,0x00,0x07,0x00,0x01,0x00,0x03,0x00,0x01,0x60,0x00
};
/*
byte getOddCheckSum(byte * data,byte len)
{
  byte odd = 0;
  for(byte i=1;i<len;i+=2)
  {
    odd = odd ^ data[i];
  }

  return odd;
}


byte getEvenCheckSum(byte * data,byte len)
{
  byte even = 0;
  for(byte i = 0;i<len;i+=2)
  {
    even = even ^ data[i];
  }

  return even;
}

void sendFrame(byte *data, byte len)
{
    byte even,odd;
    even = getEvenCheckSum(data,len);
    odd = getOddCheckSum(data,len);

    for(int i=0;i<len;i++)
    {
        while(!(UCSRA & (1<<UDRE)))
            ;

        byte c = data[i];
        UDR = c;
    }

    while(!(UCSRA & (1<<UDRE)))
            ;
    UDR = even;
    while(!(UCSRA & (1<<UDRE)))
            ;
    UDR = odd;

}
*/
int main()
{
    /*     Инициализация UART     */
    UBRRL = 8; //115200 для FBUS
    UCSRA = (1 << U2X);
	UCSRB = (0 << UDRIE) | (0 << TXCIE) | (0 << RXCIE) | (1 << RXEN) | (1 << TXEN);
    UCSRC = (1 << URSEL) | (0 << UMSEL) | (0 << USBS) | (3 << UCSZ0); //8n1
    /* */

    /*     Инициализация LCD      */

    LcdInit();

    LcdClear();
    LcdGotoXYFont(0,0);
    LcdFStr(FONT_1X,(unsigned char*)PSTR("Initialization"));
    LcdGotoXYFont(0,1);
    LcdFStr(FONT_1X,(unsigned char*)PSTR("Ok"));
    LcdUpdate();

    for (;;)
    {

    }

    return 0;
}
