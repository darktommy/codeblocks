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
#include "xprintf.h"

#define SPEED4800 103
#define SPEED9600 51
#define SPEED19200 25
#define SPEED38400 12
#define SPEED250000 1

#define BUFFER_LEN 64

#define WAIT_FOR_WRITE while (!(UCSRA & (1 << UDRE))) {}
#define WAIT_FOR_READ while (!(UCSRA & (1 << RXC))) {}
//#define serialReadBytes(buf,n) while(serialAvailable() < n) ; memcpy(buf,_buffer,n); shift(n);
//#define serialWaitUntil(terminator) while(serialLast() != terminator) {}
volatile uint8_t _serial_initialized;


byte _buffer[BUFFER_LEN];
volatile uint8_t _buffer_length;
volatile uint8_t lastChar;

//initialization
void serialInit(uint8_t SPEED); // default 8N1
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
