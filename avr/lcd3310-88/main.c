// В настройках проекта обязательно правильно укажите свою тактовую частоту
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "serial.h"

#include "n3310.h"
//#include "picture.h"


void SetupTimers()
{

    DDRD = (1<<PD6);
    OCR0A = 30;
    TCCR0A = (0 << COM0A1) | (1 << COM0A0) | (1 << WGM01) | (0 << WGM00);
    TCCR0B = (1 << CS02) | (0 << CS01) | (0 << CS00);
    //TIMSK0 = (1 << OCIE0A);
}

int main()
{

    /*     Инициализация LCD      */

    SetupTimers();

    serialInit(SPEED9600);


    LcdInit();

    LcdClear();
    LcdGotoXYFont(0,0);
    LcdFStr(FONT_1X,(unsigned char*)PSTR("Initialization"));
    LcdGotoXYFont(0,1);
    LcdFStr(FONT_1X,(unsigned char*)PSTR("Ok"));
    LcdUpdate();

    sei();

    serialWrite("Enter frequence(63-15000):\n");
    int num;
    char buf[10];
    for (;;)
    {
        serialWaitUntil('\r');
        num = serialParseInt();

        OCR0A = (15625 / num) - 1;

        serialClearBuffer();

        itoa(OCR0A, buf, 10);
        LcdClear();
        LcdGotoXYFont(0,0);
        LcdStr(FONT_1X, buf);
        LcdUpdate();



    }

    return 0;
}
