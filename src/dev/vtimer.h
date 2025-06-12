#include "stdint.h"

#ifndef VTIMER_H
#define VTIMER_H

typedef struct
{
    uint16_t counter;
    void (*callback)(void);
} Vtimer;

typedef enum
{
    Timer_0,
    Timer_1,
    Timer_2,
    Timer_3,
    Timer_4,
    Timer_5,
    Timer_6,
    Timer_7,
    Timer_8,
    Timer_9,
} VTimers;
#define VTIMER_MAX_COUNT 10

void vtimer_init(void);
void vtimer_set(VTimers timer, uint16_t ms, void (*callback)(void));
void vtimer_await(VTimers timer);
void vtimer_delay(uint16_t ms);
void vtimer_update(void);

#endif