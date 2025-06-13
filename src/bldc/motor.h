
#ifndef MOTOR_H
#define MOTOR_H

#include "stdint.h"

typedef struct
{
    uint16_t init_flag;
    uint8_t mode;
    uint16_t Dmin, Dmax;
    uint16_t Cmin, Cmax;
    uint16_t Smin, Smax;
} Params;

typedef struct
{
    Params *params;
    uint8_t status;   // run stop break
    uint8_t mode;     // voltage_mode current_mode speed_mode
    uint16_t duty;    // duty pencent in [0,1000]
    uint16_t current; // current
    uint16_t speed;   // speed
} Motor;

#endif