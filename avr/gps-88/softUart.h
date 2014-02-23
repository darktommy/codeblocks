//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru
//
//  Target(s)...: ����� AVR ����������
//
//  Compiler....: IAR 5.11A
//
//  Description.: ������� ������������ UART`a �� ����������������� AVR.
//                ��� ������� � �������������� appnote �����  IAR.
//
//  Data........: 17.01.12
//  Data........: 05.03.13   �������� ������ ��������� ���������,
//                           �������� ����������� ����������� ����������,
//                           ������� �������� ��������� ��������� � ������ ��������
//***************************************************************************
#ifndef SOFT_UART_H
#define SOFT_UART_H

#include "compilers_3.h"

/*___________________________���������_____________________________________*/

/*�������� ������� ��*/
#ifndef F_CPU
   #define F_CPU          8000000L
#endif

/*��������� ������� 1L, 8L, 64L, 256L, 1024L*/
#define SUART_PRESCALER   8L

/*�������� ������*/
#define SUART_BAUD_RATE   9600L

/*����������� ��������� ������*/
#define SUART_BUF_SIZE    64

/*��� ���������*/
#define RX_PINX       PINC
#define RX_PORTX      PORTC
#define RX_DDRX       DDRC
#define RX_PIN        0

/*��� �����������*/
#define TX_PORTX      PORTC
#define TX_DDRX       DDRC
#define TX_PIN        1

/*������� ��� ������, ����������� �� �����
�������� ����������� ������ � �������� �����.
��� ����������� � ������� SUART_GetChar()*/
#define SUART_Idle()

/*________________________���������������� �������___________________________*/

void SUART_Init(void);
char SUART_GetChar(void);
void SUART_PutChar(char ch);
void SUART_PutStr(char *str);
void SUART_PutStrFl(char __flash  *str);
void SUART_FlushInBuf(void);
char SUART_Kbhit(void);
void SUART_TurnRxOn(void);
void SUART_TurnRxOff(void);

/*___________________________________________________________________________*/

#endif //SOFT_UART_H

