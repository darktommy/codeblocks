#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"

int main(void)
{
    serialInit(SPEED9600);
    sei();
    while(1)
    {
       
    }

    return 0;
}
