#include "pwm.h"
#include "stm8s.h"

// 转子转速 10000 RPM
// 转子角频率 10000 / 60 = 166.66 Hz
// 电角频率 166.66 * PP =2.333 khz
// 六步换向速度 2.333 khz/6 = 388.88hz
// 六步换向周期 0.0025714s = 2.5714ms
void dev_pwm_init()
{
    TIM1_DeInit();
    // 16Mhz / 1000 => 16khz
    // 16Mhz / 100 => 160khz
    // 16Mhz / 160 => 100khz
    // 16Mhz / 200 => 80khz
    // 16Mhz / 300 => 53.3khz
    // 16Mhz / 400 => 40khz
    TIM1_TimeBaseInit(160 - 1, TIM1_COUNTERMODE_UP, 1000, 0);
    TIM1_OC1Init(TIM1_OCMODE_PWM1,                                   // mode
                 TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, // enable disable
                 0,                                                  // pulse width
                 TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH,        // polarity
                 TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_SET       // idle state,when OCE(TIM1_CtrlPWMOutputs)=DISABLE
    );
    TIM1_OC2Init(TIM1_OCMODE_PWM1,
                 TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE,
                 0,
                 TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH,
                 TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_SET);
    TIM1_OC3Init(TIM1_OCMODE_PWM1,
                 TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE,
                 0,
                 TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH,
                 TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_SET);
    TIM1_Cmd(ENABLE);
    TIM1_CtrlPWMOutputs(ENABLE);
}

// void enable_alternate_function()
// {
// OPT2.AFR0 = 1 # C5=TIM2_CH1; C6=TIM1_CH1； C7=TIM1_CH2；
// uint16_t OPT2 = FLASH_ReadOptionByte(&OPT->OPT2);
// if ((OPT2 & 0x0100) == 0x00)
// {
//     FLASH_Unlock(FLASH_MEMTYPE_DATA);
//     FLASH_ProgramOptionByte(&OPT->OPT2, 0x01);
//     FLASH_Lock(FLASH_MEMTYPE_DATA);
//     // WWDG_SWReset();
// }
// }

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

// CCMR
#define CCMR_PWM TIM1_OCMODE_PWM1
#define CCMR_LOW TIM1_OCMODE_INACTIVE
#define CCMR_OFF TIM1_OCMODE_INACTIVE

#define CCER1_CH1_ON 0x01
#define CCER1_CH2_ON 0x10
#define CCER2_CH3_ON 0x01
#define CCER2_CH4_ON 0x10

#define CCER1_CH1_OFF 0x00
#define CCER1_CH2_OFF 0x00
#define CCER2_CH3_OFF 0x00
#define CCER2_CH4_OFF 0x00

typedef struct
{
    uint8_t CCMR1; /*!< CC mode register 1 */
    uint8_t CCMR2; /*!< CC mode register 2 */
    uint8_t CCMR3; /*!< CC mode register 3 */
    uint8_t CCER1; /*!< CC enable register 1 */
    uint8_t CCER2; /*!< CC enable register 2 */
} CC_REG_STATE;

const CC_REG_STATE channel_steps_ccw[6] = {
    // ch1=F, ch2=H, ch3=L
    {
        .CCMR1 = CCMR_OFF,
        .CCMR2 = CCMR_PWM,
        .CCMR3 = CCMR_LOW,
        .CCER1 = CCER1_CH1_OFF | CCER1_CH2_ON,
        .CCER2 = CCER2_CH3_ON,
    },
    // ch1=L ch2=H ch3=F
    {
        .CCMR1 = CCMR_LOW,
        .CCMR2 = CCMR_PWM,
        .CCMR3 = CCMR_OFF,
        .CCER1 = CCER1_CH1_ON | CCER1_CH2_ON,
        .CCER2 = CCER2_CH3_OFF,
    },
    // ch1=L ch2=F ch3=H
    {
        .CCMR1 = CCMR_LOW,
        .CCMR2 = CCMR_OFF,
        .CCMR3 = CCMR_PWM,
        .CCER1 = CCER1_CH1_ON | CCER1_CH2_OFF,
        .CCER2 = CCER2_CH3_ON,
    },
    // ch1=F ch2=L ch3=H
    {
        .CCMR1 = CCMR_OFF,
        .CCMR2 = CCMR_LOW,
        .CCMR3 = CCMR_PWM,
        .CCER1 = CCER1_CH1_OFF | CCER1_CH2_ON,
        .CCER2 = CCER2_CH3_ON,
    },
    // ch1=H ch2=L ch3=F
    {
        .CCMR1 = CCMR_PWM,
        .CCMR2 = CCMR_LOW,
        .CCMR3 = CCMR_OFF,
        .CCER1 = CCER1_CH1_ON | CCER1_CH2_ON,
        .CCER2 = CCER2_CH3_OFF,
    },
    // ch1=H ch2=F ch3=L
    {
        .CCMR1 = CCMR_PWM,
        .CCMR2 = CCMR_OFF,
        .CCMR3 = CCMR_LOW,
        .CCER1 = CCER1_CH1_ON | CCER1_CH2_OFF,
        .CCER2 = CCER2_CH3_ON,
    },
};

void dev_pwm_set_step(uint8_t step)
{
    disableInterrupts();
    CC_REG_STATE state = channel_steps_ccw[step];
    TIM1->CCER1 = state.CCER1;
    TIM1->CCER2 = state.CCER2;
    TIM1->CCMR1 = state.CCMR1;
    TIM1->CCMR2 = state.CCMR2;
    TIM1->CCMR3 = state.CCMR3;
    enableInterrupts();
}
/**
 * pwm占空比设置
 * @param duty [0,1000]
 */
void dev_pwm_set_duty(uint16_t duty)
{

    TIM1->CCR1H = TIM1->CCR2H = TIM1->CCR3H = duty >> 8;
    TIM1->CCR1L = TIM1->CCR2L = TIM1->CCR3L = duty;
}