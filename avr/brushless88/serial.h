#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED

#define byte char
#define TRUE 1
#define FALSE 0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>


#define SPEED4800 (F_CPU/16/4800-1)
#define SPEED9600 (F_CPU/16/9600-1)
#define SPEED19200 (F_CPU/16/19200-1)
#define SPEED38400 (F_CPU/16/38400-1)
#define SPEED250000 (F_CPU/16/250000-1)


#define BUFFER_LEN 128

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega16__)
#define WAIT_FOR_WRITE while (!(UCSRA & (1 << UDRE))) {}
#define WAIT_FOR_READ while (!(UCSRA & (1 << RXC))) {}
#endif

//TODO:
#if defined(__AVR_ATmega88P__) || defined(__AVR_ATmega48__) || defined(__AVR_ATmega168__)
#define WAIT_FOR_WRITE while (!(UCSR0A & (1 << UDRE0))) {}
#define WAIT_FOR_READ while (!(UCSR0A & (1 << RXC0))) {}
#endif

volatile uint8_t _serial_initialized;


byte _buffer[BUFFER_LEN];
volatile uint8_t _buffer_length;
volatile uint8_t lastChar;

//initialization
void serialInit(uint8_t ubrr); // default 8N1
void serialEnd();

uint8_t serialAvailable();
void serialWriteChar(byte ch);
void serialWrite(byte* buf);
void serialWriteF(const unsigned char* buf); //for strings in flash rom
void serialPrintInt(uint16_t value);
void serialPrintIntLn(uint16_t value);

byte serialLast() __attribute__ ((noinline));
byte serialReadChar();
byte serialRead(byte* buf,uint8_t n);
uint8_t serialReadUntil(byte* buf, uint8_t maxlen, byte terminator);
uint8_t serialWaitUntil(byte terminator);
long serialParseInt();
float serialParseFloat();
void serialClearBuffer();

#endif // SERIAL_H_INCLUDED
