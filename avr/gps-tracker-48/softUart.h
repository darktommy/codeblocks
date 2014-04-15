//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru
//
//  Target(s)...: любой AVR контроллер
//
//  Compiler....: IAR 5.11A
//
//  Description.: Драйвер программного UART`a на микроконтроллерах AVR.
//                Код написан с использованием appnote фирмы  IAR.
//
//  Data........: 17.01.12
//  Data........: 05.03.13   исправил расчет таймерной константы,
//                           исправил неправильно объявленные переменные,
//                           добавил проверку таймерной константы и ошибки скорости
//***************************************************************************
#ifndef SOFT_UART_H
#define SOFT_UART_H

#include "compilers_3.h"

/*___________________________Настройки_____________________________________*/

/*тактовая частота мк*/
#ifndef F_CPU
   #define F_CPU          8000000L
#endif

/*прескалер таймера 1L, 8L, 64L, 256L, 1024L*/
#define SUART_PRESCALER   8L

/*скорость обмена*/
#define SUART_BAUD_RATE   9600L

/*вместимость приемного буфера*/
#define SUART_BUF_SIZE    80

/*пин приемника*/
#define RX_PINX       PINB
#define RX_PORTX      PORTB
#define RX_DDRX       DDRB
#define RX_PIN        1

/*пин передатчика*/
#define TX_PORTX      PORTB
#define TX_DDRX       DDRB
#define TX_PIN        0

/*функция или макрос, выполняемый во время
ожидания поступления данных в приемный буфер.
она запускается в функции SUART_GetChar()*/
#define SUART_Idle()

/*________________________Пользовательские функции___________________________*/

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

