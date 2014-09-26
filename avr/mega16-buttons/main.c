#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "buttons.h"
#include <ctype.h>
#include "xprintf.h"

uint8_t convertTo7bit(char* inStr, char* outStr)
{
    uint16_t msgLen = strlen(inStr);
    uint16_t outMsgLen = 0;
    if (msgLen % 8 != 0)
        outMsgLen = msgLen * 7 / 8 +1;
    else
        outMsgLen = msgLen * 7 / 8;

    uint8_t c, c1,c2;
    uint8_t i,ii,k;
    i = 0;
    ii = 0;
    k=0;

    while(k < outMsgLen)
    {
        c1 = inStr[i];
        c2 = inStr[i+1];
        c = c1 >> ii;
        c &= ~(0xFF << (7 - ii));
        c2 <<= (7 - ii);
        c |= c2;
        ii++;

        if (ii == 7)
        {
            ii = 0;
            i++;
        }
        outStr[k] = c;
        k++;
        i++;
    }
    outStr[k] = 0;

    return outMsgLen;
}

void sendSMS(uint8_t *recipient, uint8_t *message) //only latin symbols
{
    uint8_t pduStr[200];
    memset(pduStr,0,200);

    strcat(pduStr, "00");
    strcat(pduStr, "01");      // PDU-Type
    strcat(pduStr, "00");      // TP-Message-Reference

/*
    Заполнение поля TP-Destination-Adress
*/
    uint8_t recv[25];
    uint8_t recv_len=0;
    uint8_t tpda_len=0;
    uint8_t tpda_len_str[3];
    memset(recv, 0, 25);

    uint8_t i;
    for (i=0;i<strlen(recipient);i++)
        if (isdigit(recipient[i]))
            recv[recv_len++] = recipient[i];

    tpda_len = recv_len;

    if (recv_len % 2 == 1)
    {
        strcat(recv, "F");
        recv_len++;
    }

    for (i=0;i<recv_len;i+=2)
    {
        uint8_t c;
        c = recv[i];
        recv[i] = recv[i+1];
        recv[i+1] = c;
    }
    xsprintf(tpda_len_str, "%02X", tpda_len);

    strcat(pduStr, tpda_len_str);        // TP-Destination-Adress
    strcat(pduStr, "91");
    strcat(pduStr, recv);

/* */

    strcat(pduStr, "00");      // TP-Protocol-ID
    strcat(pduStr, "00");        // TP-Data-Coding-System

    //strcat(pduStr, "00");        // TP-Validity-Period

/*
    7-битная перекодировка
*/
    uint8_t buf2[3];

    uint8_t msgLen = strlen(message);
    xsprintf(buf2, "%02X", msgLen);
    strcat(pduStr, buf2);        // TP-User-Data-Length

    uint16_t outMsgLen = 0;
    if (msgLen % 8 != 0)
        outMsgLen = msgLen * 7 / 8 +1;
    else
        outMsgLen = msgLen * 7 / 8;

    uint8_t c, c1,c2;
    uint8_t ii,k;

    i = 0;
    ii = 0;
    k=0;

    while(k < outMsgLen)
    {
        c1 = message[i];
        c2 = message[i+1];
        c = c1 >> ii;
        c &= ~(0xFF << (7 - ii));
        c2 <<= (7 - ii);
        c |= c2;
        ii++;

        if (ii == 7)
        {
            ii = 0;
            i++;
        }

        xsprintf(buf2,"%02X", c);
        strcat(pduStr, buf2);

        k++;
        i++;
    }

    serialWrite(pduStr);

/* */

}

void OnClick0()
{
    uint8_t msg[] = "How are you?";
    uint8_t recipient[] = "+31641600986";
    uint8_t pdu[160];
    sendSMS(recipient, msg);

}

void OnClick1()
{
    serialWrite("Button2 Clicked\n");
}

int main(void)
{
    DDRC = 0x00;//PORTC на вход
    PORTC = 0x3;


    ButtonsInit();
    ButtonsSet(&PINC, PIN0, 1, &OnClick0);
    ButtonsSet(&PINC, PIN1, 1, &OnClick1);
    ButtonsActivate();

    serialInit(SPEED9600);
    sei();


    serialWrite("Hello! AVR Mega16 Boot OK\n");
    while(1)
    {
    }

    return 0;
}
