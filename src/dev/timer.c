#include "timer.h"
#include "clock.h"
// 16Mhz / 16 => 1us ; arr=>1000 ; ovf => 1ms ;
// 16Mhz / 32 => 2us ; arr=>500 ; ovf => 1ms ;
// 16Mhz / 64 => 4us ; arr=>250 ; ovf => 1ms ;
// 16Mhz / 128 => 8us ; arr=>125 ; ovf => 1ms ;

// 24Mhz / 16 => 0.66us ; arr=>1515 ; ovf => 1ms ;
// 24Mhz / 32 => 1.32us ; arr=>757.5 ; ovf => 1ms ;
// 24Mhz / 64 => 2.64us ; arr=>378.75 ; ovf => 1ms ;
// 24Mhz / 128 => 5.28us ; arr=>189.375 ; ovf => 1ms ;
// Arr = (Freq/Prescaler)/1ms = (Freq/Prescaler)/1 * 1000

#define DELTAT_MS 1
#define ArrValue ((DELTAT_MS * STM8_FREQ_MHZ * 1000) / 128)

void dev_timer_init()
{
    TIM4_DeInit();
    /* Time base configuration */
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, ArrValue);
    ITC_SetSoftwarePriority(ITC_IRQ_TIM4_OVF, ITC_PRIORITYLEVEL_1);

    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    enableInterrupts(); // 打开总中断
    // disableInterrupts(); // 关闭总中断

    dev_timer_clear();
    
    /* Enable TIM4 */
    TIM4_Cmd(ENABLE);
}
/**
 * @brief Timer4 Update/Overflow Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
    dev_timer_update();
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
}

static Vtimer vtimers[VTIMER_MAX_COUNT];

void dev_timer_clear(void)
{
    for (uint8_t i = 0; i < VTIMER_MAX_COUNT; i++)
    {
        vtimers[i].counter = 0;
        vtimers[i].callback = 0;
    }
}

void dev_timer_set(VTimers timer, uint16_t ms, void (*callback)(void))
{
    vtimers[timer].counter = ms;
    vtimers[timer].callback = callback;
}

void dev_timer_await(VTimers timer)
{
    while (vtimers[timer].counter != 0)
        ;
}
void dev_timer_delay(uint16_t ms)
{
    dev_timer_set(Timer_9, ms, 0);
    dev_timer_await(Timer_9);
}
void dev_timer_update(void)
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