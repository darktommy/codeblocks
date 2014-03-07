
#include "gsm.h"

uint8_t GSM_WaitResponse(char *expectedStr)
{
    char buf[64];
    uint8_t buflen=0;

    while(!(buflen = serialAvailable()))
        ;

    serialRead(buf, buflen);
    buf[buflen] = '\0';

    if(strstr(buf, expectedStr))
        return 1;
    return 0;
}

uint8_t GSM_Test()
{
    serialWrite("AT\r");
    _delay_ms(100);
    uint8_t len;
    char buf[40];
    len = serialAvailable();
    if(len)
    {
        serialRead(buf, len);
        buf[len] = 0;
        if(strstr(buf, "OK"))
            return 1;
    }
    return 0;
}

void GSM_Init()
{
    serialWriteF((const unsigned char*)PSTR("ATE0\r"));
    _delay_ms(500);
    serialWriteF((const unsigned char*)PSTR("AT+CMGF=1\r"));//text mode
    _delay_ms(500);
    serialWriteF((const unsigned char*)PSTR("AT+CLIP=1\r")); //включает АОН
    _delay_ms(500);
    //serialWriteF((const unsigned char*)PSTR("AT+IFC=1\r")); //flow control
    //_delay_ms(300);
    //serialWriteF((const unsigned char*)PSTR("AT+CPBS=\"SM\"\r")); //
    //_delay_ms(300);
    //serialWriteF((const unsigned char*)PSTR("AT+CNMI=1,2,2,1,0\r")); //чето там настраивается, сообщения походу не сохраняются. ну и ладно :)
    //_delay_ms(300);
    serialClearBuffer();
}

void GSM_PowerOn()
{
    DDRC |= (1 << PC2);

    PORTC |= (1 << PC2);
    _delay_ms(2000);

    PORTC &= ~(1 << PC2);
    _delay_ms(2000);


}

uint8_t GSM_WaitResponse2(char* expectedStr)
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

uint8_t GSM_Test2()
{
    SUART_PutStrFl((char*)PSTR("AT\r"));
    char buf[20];
    uint8_t buflen=0;
    _delay_ms(100);
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

void GSM_Init2()
{
    SUART_PutStrFl((char*)PSTR("ATE0\r")); //echo off
    _delay_ms(300);
    SUART_PutStrFl((char*)PSTR("AT+CMGF=1\r"));//text mode
    _delay_ms(300);
    SUART_PutStrFl((char*)PSTR("AT+CLIP=1\r")); //включает АОН
    _delay_ms(500);

    SUART_FlushInBuf();

}


