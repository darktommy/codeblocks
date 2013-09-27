/*
 */

#include <avr/io.h>
#include "serial.h"

int main(void)
{
    serialInit(SPEED9600);
    sei();

    serialWrite("HEllo world!!!\n");
    while(1)
    {
        uint8_t a,b,c;
        serialWaitUntil('\r');
        a = serialParseInt();
        b = serialParseInt();
        c = serialParseInt();

        serialPrintIntLn(a*b*c);
        serialClearBuffer();
    }
    serialEnd();

    return 0;
}
