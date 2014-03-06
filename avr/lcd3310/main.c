// В настройках проекта обязательно правильно укажите свою тактовую частоту
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "n3310.h"
//#include "picture.h"
#include "serial.h"
int main()
{

    /*     Инициализация LCD      */
    serialInit(SPEED9600);
    LcdInit();

    sei();

    LcdClear();
    LcdGotoXYFont(0,0);
    LcdFStr(FONT_1X,(unsigned char*)PSTR("Initialization"));
    LcdGotoXYFont(0,1);
    LcdFStr(FONT_1X,(unsigned char*)PSTR("Ok"));
    LcdUpdate();

    LcdGotoXYFont(0,0);
    for (;;)
    {
        serialWrite("AT\r");
        _delay_ms(100);
        uint8_t len = serialAvailable();
        char buf[60];
        serialRead(buf,len);
        buf[len] = '\0';

        LcdStr(FONT_1X, (byte*)buf);
        LcdUpdate();


    }

    return 0;
}
