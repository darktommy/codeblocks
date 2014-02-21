// В настройках проекта обязательно правильно укажите свою тактовую частоту
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "serial.h"

#include "n3310.h"
#include "audio.h"
//#include "picture.h"

uint16_t duration_cnt;
uint8_t position;

uint16_t melody[]=
{
E5,d16,
D5,d16,
F4d,d8,
G4d,d8,
C5d,d16,
B4,d16,
D4,d8,
E4,d8,
B4,d16,
A4,d16,
C4d,d8,
E4,d8,
A4,d4,
PAUSE,d2,
PAUSE,d2,
END
};



void ChangeTone()
{
    position += 2;

    if(position >= sizeof(melody))
        position = 0;

    if(melody[position] == END)
    {
        position = 0;
    }

    if(melody[position] == PAUSE)
    {
        DDRB &= ~(1 << PB1);
    }
    else
    {
        DDRB |= (1 << PB1);
    }

    TCNT1 = 0;
    OCR1A = melody[position];
    duration_cnt = durations[melody[position+1]];
}

ISR(TIMER0_COMPA_vect)
{
    if(duration_cnt == 0)
    {
        ChangeTone();
        return;
    }

    duration_cnt--;
}

void AudioInit()
{
    OCR1A = melody[position];
    duration_cnt = durations[melody[position+1]];
}

void SetupTimers()
{

    DDRB = (1<<PB1);//OC1A as output

    //timer0
    OCR0A = 49;//interruption every 0.4 ms
    TCCR0A = (1 << WGM01) | (0 << WGM00); //CTC-mode
    TCCR0B = (1 << CS01) | (1 <<CS00); // prescaler = 64
    TIMSK0 = (1 << OCIE0A);
    TCNT0 = 0;

    //timer1
    TCCR1A = (0 << COM1A1) | ( 1 << COM1A0); //toggle OC1A on compare-match
    TCCR1B = (1 << WGM12) | (1 << CS11); // prescaler = 8, CTC-mode
    TCNT1 = 0;

}

int main()
{

    /*     Инициализация LCD      */

    SetupTimers();
    AudioInit();
    serialInit(SPEED9600);

    sei();


    while(1)
    {

    }
/*

    LcdInit();

    LcdClear();
    LcdGotoXYFont(0,0);
    LcdFStr(FONT_1X,(unsigned char*)PSTR("Initialization"));
    LcdGotoXYFont(0,1);
    LcdFStr(FONT_1X,(unsigned char*)PSTR("Ok"));
    LcdUpdate();

    sei();

    serialWrite("Enter frequence(63-15000):\n");
    int num;
    char buf[10];
    for (;;)
    {
        serialWaitUntil('\r');
        num = serialParseInt();

        OCR1A = (500000 / num) - 1;

        serialClearBuffer();

        itoa(OCR1A, buf, 10);
        LcdClear();
        LcdGotoXYFont(0,0);
        LcdStr(FONT_1X, buf);
        LcdUpdate();



    }
*/
    return 0;
}
