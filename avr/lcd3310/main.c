// В настройках проекта обязательно правильно укажите свою тактовую частоту
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "n3310.h"
//#include "picture.h"

int main()
{

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
