#include "pwm.h"
#include "stm8s.h"
void dev_pwm_init()
{
    TIM1_DeInit();
    // 16Mhz / 1000 => 16khz
    TIM1_TimeBaseInit(1000 - 1, TIM1_COUNTERMODE_UP, 1000, 0);
    TIM1_CtrlPWMOutputs(ENABLE);
}

// step A B C
//  1   F H L
//  2   L H F
//  3   L F H
//  4   F L H
//  5   H L F
//  6   H F L

#define H 0
#define L 1
#define F 2

typedef struct
{
    uint8_t ch1;
    uint8_t ch2;
    uint8_t ch3;
} Step;

const Step channel_steps_ccw[6] = {
    {F, H, L},
    {L, H, F},
    {L, F, H},
    {F, L, H},
    {H, L, F},
    {H, F, L},
};

void dev_pwm_set_step(uint8_t step_idx)
{
    Step step = channel_steps_ccw[step_idx];
    switch (step.ch1)
    {
    case H:
        break;
    case L:
        break;
    case F:
        break;
    };
    switch (step.ch2)
    {
    case H:
        break;
    case L:
        break;
    case F:
        break;
    };
    switch (step.ch3)
    {
    case H:
        break;
    case L:
        break;
    case F:
        break;
    };
    // if (step == 1)
    // {
    //     /* Next step: Step 2 Configuration ---------------------------- */
    //     /*  Channel3 configuration */
    //     TIM1_CCxCmd(TIM1_CHANNEL_3, DISABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_3, DISABLE);

    //     /*  Channel1 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_1, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_ACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_1, ENABLE);

    //     /*  Channel2 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_2, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_INACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_2, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_2, ENABLE);
    //     step++;
    // }
    // else if (step == 2)
    // {
    //     /* Next step: Step 3 Configuration ---------------------------- */
    //     /*  Channel2 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_2, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_INACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_2, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_2, ENABLE);

    //     /*  Channel3 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_3, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_ACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_3, ENABLE);

    //     /*  Channel1 configuration */
    //     TIM1_CCxCmd(TIM1_CHANNEL_1, DISABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_1, DISABLE);
    //     step++;
    // }
    // else if (step == 3)
    // {
    //     /* Next step: Step 4 Configuration ---------------------------- */
    //     /*  Channel3 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_3, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_ACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_3, ENABLE);

    //     /*  Channel2 configuration */
    //     TIM1_CCxCmd(TIM1_CHANNEL_2, DISABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_2, DISABLE);

    //     /*  Channel1 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_1, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_INACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_1, ENABLE);
    //     step++;
    // }
    // else if (step == 4)
    // {
    //     /* Next step: Step 5 Configuration ---------------------------- */
    //     /*  Channel3 configuration */
    //     TIM1_CCxCmd(TIM1_CHANNEL_3, DISABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_3, DISABLE);

    //     /*  Channel1 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_1, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_INACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_1, ENABLE);

    //     /*  Channel2 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_2, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_ACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_2, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_2, ENABLE);
    //     step++;
    // }
    // else if (step == 5)
    // {
    //     /* Next step: Step 6 Configuration ---------------------------- */
    //     /*  Channel3 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_3, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_INACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_3, ENABLE);

    //     /*  Channel1 configuration */
    //     TIM1_CCxCmd(TIM1_CHANNEL_1, DISABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_1, DISABLE);

    //     /*  Channel2 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_2, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_ACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_2, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_2, ENABLE);
    //     step++;
    // }
    // else
    // {
    //     /* Next step: Step 1 Configuration ---------------------------- */
    //     /*  Channel1 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_1, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_ACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_1, ENABLE);

    //     /*  Channel3 configuration */
    //     TIM1_SelectOCxM(TIM1_CHANNEL_3, (TIM1_OCMode_TypeDef)TIM1_FORCEDACTION_INACTIVE);
    //     TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_3, ENABLE);

    //     /*  Channel2 configuration */
    //     TIM1_CCxCmd(TIM1_CHANNEL_2, DISABLE);
    //     TIM1_CCxNCmd(TIM1_CHANNEL_2, DISABLE);
    //     step = 1;
    // }

}
/**
 * pwm占空比设置
 * @param duty [0,1000]
 */
void dev_pwm_set_duty(uint16_t duty)
{

    TIM1->CCR1H = TIM1->CCR2H = TIM1->CCR3H = duty << 8;
    TIM1->CCR1L = TIM1->CCR2L = TIM1->CCR3L = duty;
}