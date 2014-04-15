
#include "gsm.h"

void GSM_PowerOn()
{
    POWERKEY_DDRX |= (1 << POWER_PIN);

    POWERKEY_PORTX |= (1 << POWER_PIN);
    _delay_ms(2000);

    POWERKEY_PORTX &= ~(1 << POWER_PIN);
    _delay_ms(2000);

}

uint8_t GSM_WaitResponse(char* expectedStr)
{
    char buf[100];
    uint8_t buflen=0;
    while(SUART_Kbhit())
        buf[buflen++] = SUART_GetChar();

    buf[buflen] = '\0';
    if(strstr(buf, expectedStr) != NULL)
        return 1;

    return 0;
}

uint8_t GSM_Test()
{
    SUART_PutStrFl((char*)PSTR(" AT\r"));
    char buf[20];
    uint8_t buflen=0;
    _delay_ms(500);
    while(SUART_Kbhit())
    {
        buf[buflen] = SUART_GetChar();
        buflen++;
    }
    buf[buflen] = '\0';

    SUART_FlushInBuf();

    if(strstr(buf,"OK") != NULL)
        return 1;

    return 0;

}

void GSM_Init()
{
    SUART_PutStrFl((char*)PSTR(" ATE0\r")); //echo off
    _delay_ms(1000);
    SUART_FlushInBuf();

    SUART_PutStrFl((char*)PSTR(" AT+CMGF=1\r"));//text mode
    _delay_ms(1000);
    SUART_FlushInBuf();

    SUART_PutStrFl((char*)PSTR(" AT+CLIP=1\r")); //включает АОН
    _delay_ms(1000);
    SUART_FlushInBuf();
    //SUART_PutStrFl((char*)PSTR(" AT+CSCLK=2\r")); //sleep mode 2
    //_delay_ms(300);

}


