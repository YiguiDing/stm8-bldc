#include "led.h"
#include "stm8s.h"
void dev_led_init()
{
    GPIO_DeInit(GPIOB);
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
}

void dev_led_on()
{
    GPIO_WriteLow(GPIOB, GPIO_PIN_5);
}

void dev_led_off()
{
    GPIO_WriteHigh(GPIOB, GPIO_PIN_5);
}
