#include <avr/io.h>
#include "serial.h"
#include "mirf.h"

int main(void)
{
    uint8_t buf[10];

    DDRD |= ((1 << PD5) | (1 << PD6));//PD5 и PD6 на выход

    serialInit(SPEED9600);
    mirf_init();
    _delay_ms(50);
    sei();
    mirf_config();

    while(1)
    {
        while(!mirf_data_ready()) ;

        mirf_get_data(buf);
        if(buf[0] == 1)
        {
            PORTD |= (1 << PD5);
        }
        else
        {
            PORTD &= ~(1 << PD5);
        }

        if(buf[1] == 1)
        {
            PORTD |= (1 << PD6);
        }
        else
        {
            PORTD &= ~(1 << PD6);
        }
    }

    return 0;
}
