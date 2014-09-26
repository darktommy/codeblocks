#ifndef _BUTTONS_
#define _BUTTONS_

#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"

#define BUTTONS_COUNT_MAX 4

void ButtonsInit();
void ButtonsSet(volatile uint8_t *port, uint8_t pin, uint8_t flag, void (*func));
void ButtonsActivate();

#endif // _BUTTONS_
