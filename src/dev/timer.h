#ifndef TIMER_H
#define TIMER_H

#include "stm8s.h"

#define VTIMER_MAX_COUNT 10

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


void dev_timer_init();
void dev_timer_clear();
void dev_timer_set(VTimers timer, uint16_t ms, void (*callback)(void));
void dev_timer_await(VTimers timer);
void dev_timer_delay(uint16_t ms);
void dev_timer_update(void);

#endif