#include "dev.h"
#include "timer.h"
#include "clock.h"
#include "pwm.h"
#include "led.h"
void dev_init()
{
    dev_clock_init();
    dev_pwm_init();
    dev_timer_init();
    dev_led_init();
}