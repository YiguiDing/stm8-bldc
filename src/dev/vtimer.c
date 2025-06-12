
#include "vtimer.h"

static Vtimer vtimers[VTIMER_MAX_COUNT];

void vtimer_init(void)
{
    for (uint8_t i = 0; i < VTIMER_MAX_COUNT; i++)
    {
        vtimers[i].counter = 0;
        vtimers[i].callback = 0;
    }
}

void vtimer_set(VTimers timer, uint16_t ms, void (*callback)(void))
{
    vtimers[timer].counter = ms;
    vtimers[timer].callback = callback;
}

void vtimer_await(VTimers timer)
{
    while (vtimers[timer].counter != 0)
        ;
}
void vtimer_delay(uint16_t ms)
{
    vtimer_set(Timer_9, ms, 0);
    vtimer_await(Timer_9);
}
void vtimer_update(void)
{
    for (uint8_t i = 0; i < VTIMER_MAX_COUNT; i++)
    {
        if (vtimers[i].counter)
        {
            vtimers[i].counter--;
            if (vtimers[i].callback)
                vtimers[i].callback();
        }
    }
}