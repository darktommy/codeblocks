/*
 */

#include <avr/io.h>
#include "serial.h"
#include "mirf.h"

int main(void)
{

    // Insert code
    serialInit(SPEED9600);
    mirf_init();
    _delay_ms(50);
    sei();

    mirf_config();

    uint8_t buf[10];
    serialWrite("hello max!waiting for commands.\n");
    while(1)
    {
        if(serialAvailable()== 6)
        {
            uint8_t angle;
            int16_t gas;
            uint16_t checksum;
            uint8_t ending;
            angle = serialReadChar();
            serialRead((byte*) &gas, 2);
            serialRead((byte*) &checksum, 2);
            ending = serialReadChar();

            serialClearBuffer();

            if(ending != 0x4f)
            {
                serialWrite("hmm, length is not 6, you are mistaken!\n");
                continue;
            }
            if( ((uint16_t)gas ^ (uint16_t)angle) != checksum)
            {
                serialWrite("checksum incorrent :(\n");
                continue;
            }

            buf[0] = angle;
            memcpy(buf+1,(void*)&gas,2);
            memcpy(buf+3,(void*)&checksum,2);
            buf[5] = ending;

            mirf_send(buf,6);

        }

    }


    serialEnd();

    return 0;
}
