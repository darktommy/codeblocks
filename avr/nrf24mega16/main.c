#include <avr/io.h>
#include "serial.h"
#include "mirf.h"

int main(void)
{
    uint8_t buf[10];

    serialInit(SPEED9600);
    mirf_init();
    _delay_ms(50);
    sei();
    mirf_config();

    memset(buf,1,6);

    while(1)
    {
        if(serialAvailable()>0)
        {
            char ch;
            ch = serialReadChar();
            if(ch == '1')
            {
                buf[0] = ~buf[0];
                mirf_send(buf,6);
            }
            else if(ch == '2')
            {
                buf[1] = ~buf[1];
                mirf_send(buf,6);
            }

            serialClearBuffer();
        }
    }

    return 0;
}
