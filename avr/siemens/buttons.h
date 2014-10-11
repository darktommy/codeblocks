#ifndef _BUTTONS_
#define _BUTTONS_

#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"

#define BUTTONS_COUNT_MAX 2

//ATMEGA48SERIES for ATMega*8, ATMEGA8SERIES for ATMega8/16
#define ATMEGA48SERIES

void ButtonsInit();
void ButtonsSet(volatile uint8_t *port, uint8_t pin, uint8_t flag, void (*func));
void ButtonsActivate();

#endif // _BUTTONS_
