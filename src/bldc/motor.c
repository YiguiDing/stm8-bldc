
#include "motor.h"
#include "stdint.h"

void motor_init(Motor *motor)
{
    if (motor->params->init_flag != 0x55AA)
    {
        // load default params;
        // save params to flash;
    }
    else
    {
    }
    motor->status = 0;
    motor->mode = motor->params->mode;
    motor->duty = 0;
    motor->current = 0;
    motor->speed = 0;
}

void motor_run(Motor *motor)
{
    motor->status = 1;
}

void motor_stop(Motor *motor)
{
    motor->status = 0;
}
void motor_set_mode(Motor *motor, uint8_t mode)
{
    motor->mode = mode;
}
void motor_set_duty(Motor *motor, uint16_t duty)
{
    motor->duty = duty;
}
void motor_update(Motor *motor)
{
}