#ifndef GSM_H_INCLUDED
#define GSM_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>

#include "compilers_3.h"
#include "serial.h"
#include "softUart.h"

#define POWERKEY_DDRX       DDRD
#define POWERKEY_PORTX      PORTD
#define POWER_PIN           7

uint8_t GSM_WaitResponse(char *expectedStr);
uint8_t GSM_Test();
void GSM_Init();
void GSM_PowerOn();

#endif // GSM_H_INCLUDED
