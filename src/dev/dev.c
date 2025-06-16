#include "dev.h"
#include "clock.h"
#include "timer.h"
#include "pwm.h"
#include "serial.h"
#include "led.h"
void dev_init()
{
    dev_clock_init();
    dev_timer_init();
    dev_pwm_init();
    dev_serial_init();
    dev_led_init();
}