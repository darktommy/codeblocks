//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru
//
//  Target(s)...: ATMega
//
//  Compiler....: IAR, GCC, CodeVision
//
//  Description.: файл для портирования проектов, исходников библиотек
//
//  Data........: 20.04.13
//
//***************************************************************************

#ifndef COMPILERS_H
#define COMPILERS_H

/*****************************************************************************/

#ifdef  __ICCAVR__

#include <ioavr.h>
#include <inavr.h>
#include <intrinsics.h>

#define STRINGIFY(a) #a
#define ISR(vect) _Pragma(STRINGIFY(vector = vect))\
                  __interrupt void vect##_func(void)
#define INLINE _Pragma(STRINGIFY(inline = forced))

#define read_byte_flash(x) (x)
#define read_word_flash(x) (x)
#endif

/*****************************************************************************/

#ifdef  __GNUC__

#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define __save_interrupt()       SREG
#define __restore_interrupt(var) SREG = (var)
#define __disable_interrupt()    cli()
#define __enable_interrupt()     sei()
#define __delay_cycles(var)      _delay_us((unsigned int)(var)/(F_CPU/1000000))

#define __flash
#define read_byte_flash(x) pgm_read_byte(&(x))
#define read_word_flash(x) pgm_read_word(&(x))

#define INLINE
#endif

/*****************************************************************************/

#ifdef __CODEVISIONAVR__

#include <io.h>
#include <delay.h>

#define __save_interrupt()       SREG
#define __restore_interrupt(var) SREG = (var)
#define __disable_interrupt()    #asm("cli")
#define __enable_interrupt()     #asm("sei")
#define __delay_cycles(var)      delay_us((unsigned int)(var)/(_MCU_CLOCK_FREQUENCY_/1000000))

#define read_byte_flash(x) (x)
#define read_word_flash(x) (x)

#define ISR(vect) interrupt [vect] void vect##_func(void)
#define INLINE
#endif

/*****************************************************************************/

#include <stdint.h>

#endif //COMPILERS_H

