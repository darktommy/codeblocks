#include "buttons.h"

volatile uint8_t buttonClicked[BUTTONS_COUNT_MAX];
volatile uint8_t buttonWasFirstClick[BUTTONS_COUNT_MAX];
void (*onClick[BUTTONS_COUNT_MAX])();
volatile uint8_t *ports[BUTTONS_COUNT_MAX];
uint8_t pins[BUTTONS_COUNT_MAX];
uint8_t flags[BUTTONS_COUNT_MAX];

volatile uint8_t buttonsEnabled;
volatile uint8_t buttonsCount;

ISR(TIMER0_COMP_vect)
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
        if (port)
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
    OCR0 = 77;
    TCCR0 = (1 << WGM01) | (1 << CS02) | (1 << CS00);
    TIMSK = (1 << OCIE0);

    buttonsEnabled = 1;
}
