#include "buttons.h"

volatile uint8_t buttonClicked[BUTTONS_COUNT_MAX];
volatile uint8_t buttonWasFirstClick[BUTTONS_COUNT_MAX];
void (*onClick[BUTTONS_COUNT_MAX])();
volatile uint8_t *ports[BUTTONS_COUNT_MAX];
uint8_t pins[BUTTONS_COUNT_MAX];
uint8_t flags[BUTTONS_COUNT_MAX];

volatile uint8_t buttonsEnabled;
volatile uint8_t buttonsCount;

ISR(TIMER2_OVF_vect)
{
    if (!buttonsEnabled) return;
    uint8_t i;
    for (i=0;i<buttonsCount;i++)
    {
        if(!(*ports[i] & (1 << pins[i])))
        {
            if (!buttonWasFirstClick[i])
            {
                buttonWasFirstClick[i] = 1;
                continue;
            }
            else
            {
                if (!buttonClicked[i])
                {
                    buttonClicked[i] = 1;
                    if (onClick[i])
                        (*onClick[i])();
                }
            }
        }
        else
        {
            if(buttonClicked[i])
            {
                buttonClicked[i] = 0;
                buttonWasFirstClick[i] = 0;
            }
        }
    }
}

void ButtonsInit()
{
    int i;
    for (i=0;i<BUTTONS_COUNT_MAX;i++)
    {
        buttonClicked[i] = 0;
        buttonWasFirstClick[i] = 0;
        onClick[i] = 0;
        ports[i] = 0;
        pins[i] = 0;
        flags[i] = 0;
    }
    buttonsCount = 0;
    //TODO
}

void ButtonsSet(volatile uint8_t *port, uint8_t pin, uint8_t flag, void (*func))
{
    if(buttonsCount < BUTTONS_COUNT_MAX)
    {
        onClick[buttonsCount] = func;
        ports[buttonsCount] = port;
        pins[buttonsCount] = pin;
        flags[buttonsCount] = flag;
        buttonsCount++;
    }

}

void ButtonsActivate()
{
    //SETUP TIMER0
    #ifdef ATMEGA8SERIES
    OCR0 = 77;
    TCCR0 = (1 << WGM01) | (1 << CS02) | (1 << CS00);
    TIMSK |= (1 << OCIE0);
    #endif // ATMEGA8SERIES

    #ifdef ATMEGA48SERIES
    //OCR2A = 77;
    //TCCR2A = (1 << WGM21); // CTC
    TCNT2 = 0;
    TCCR2B = (1 << CS22) || (1 << CS21);
    TIMSK2 |= (1 << TOIE2);
    #endif // ATMEGA48SERIES
    buttonsEnabled = 1;
}
