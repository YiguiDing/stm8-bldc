#ifndef PWM_H
#define PWM_H

#include "stdint.h"

void dev_pwm_init();
void dev_pwm_set_duty(uint16_t duty);
void dev_pwm_set_step(uint8_t step);

#endif