#include "pwm.h"
#include "stm8s.h"

// 转子转速 10000 RPM
// 转子角频率 10000 / 60 = 166.66 Hz
// 电角频率 166.66 * PP =2.333 khz
// 六步换向速度 2.333 khz/6 = 388.88hz
// 六步换向周期 0.0025714s = 2.5714ms
void dev_pwm_init()
{
    GPIO_DeInit(PORT);
    GPIO_Init(PORT, GPIO_CH1_L, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(PORT, GPIO_CH2_L, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(PORT, GPIO_CH3_L, GPIO_MODE_OUT_PP_LOW_FAST);
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

typedef struct
{
    uint8_t CCMR1; /*!< CC mode register 1 */
    uint8_t CCMR2; /*!< CC mode register 2 */
    uint8_t CCMR3; /*!< CC mode register 3 */
    uint8_t CCER1; /*!< CC enable register 1 */
    uint8_t CCER2; /*!< CC enable register 2 */
} CC_REG_STATE;

// CCMR
#define CCMR_PWM TIM1_OCMODE_PWM1     // pwm 模式
#define CCMR_LOW TIM1_OCMODE_INACTIVE // 强制拉低
#define CCMR_OFF TIM1_OCMODE_INACTIVE //

// CCER1
#define CCER1_CH1_ON 0x01 // 通道开关
#define CCER1_CH1_OFF 0x00
#define CCER1_CH2_ON 0x10 // 通道开关
#define CCER1_CH2_OFF 0x00
#define CCER2_CH3_ON 0x01 // 通道开关
#define CCER2_CH3_OFF 0x00
#define CCER2_CH4_ON 0x10 // 通道开关
#define CCER2_CH4_OFF 0x00

// ch1=F ch2=H ch3=L
// ch1=L ch2=H ch3=F
// ch1=L ch2=F ch3=H
// ch1=F ch2=L ch3=H
// ch1=H ch2=L ch3=F
// ch1=H ch2=F ch3=L
const CC_REG_STATE cc_steps_ccw[6] = {
    {
        .CCMR1 = CCMR_OFF,
        .CCMR2 = CCMR_PWM,
        .CCMR3 = CCMR_LOW,
        .CCER1 = CCER1_CH1_OFF | CCER1_CH2_ON,
        .CCER2 = CCER2_CH3_ON,
    },
    {
        .CCMR1 = CCMR_LOW,
        .CCMR2 = CCMR_PWM,
        .CCMR3 = CCMR_OFF,
        .CCER1 = CCER1_CH1_ON | CCER1_CH2_ON,
        .CCER2 = CCER2_CH3_OFF,
    },
    {
        .CCMR1 = CCMR_LOW,
        .CCMR2 = CCMR_OFF,
        .CCMR3 = CCMR_PWM,
        .CCER1 = CCER1_CH1_ON | CCER1_CH2_OFF,
        .CCER2 = CCER2_CH3_ON,
    },
    {
        .CCMR1 = CCMR_OFF,
        .CCMR2 = CCMR_LOW,
        .CCMR3 = CCMR_PWM,
        .CCER1 = CCER1_CH1_OFF | CCER1_CH2_ON,
        .CCER2 = CCER2_CH3_ON,
    },
    {
        .CCMR1 = CCMR_PWM,
        .CCMR2 = CCMR_LOW,
        .CCMR3 = CCMR_OFF,
        .CCER1 = CCER1_CH1_ON | CCER1_CH2_ON,
        .CCER2 = CCER2_CH3_OFF,
    },
    {
        .CCMR1 = CCMR_PWM,
        .CCMR2 = CCMR_OFF,
        .CCMR3 = CCMR_LOW,
        .CCER1 = CCER1_CH1_ON | CCER1_CH2_OFF,
        .CCER2 = CCER2_CH3_ON,
    },
};

typedef struct
{
    uint8_t ODR;
} IO_REG_STATE;

#define PORT GPIOA
#define GPIO_CH1_L GPIO_PIN_1
#define GPIO_CH2_L GPIO_PIN_2
#define GPIO_CH3_L GPIO_PIN_3
// ODR 寄存器控制 高/低电平（输出模式）
#define ODR_CH1_HI GPIO_CH1_L
#define ODR_CH1_LO ((uint8_t)0x00)
#define ODR_CH2_HI GPIO_CH2_L
#define ODR_CH2_LO ((uint8_t)0x00)
#define ODR_CH3_HI GPIO_CH3_L
#define ODR_CH3_LO ((uint8_t)0x00)

const IO_REG_STATE io_steps_ccw[6] = {
    // ch1=F ch2=H ch3=L
    {
        .ODR = ODR_CH1_LO | ODR_CH2_LO | ODR_CH3_HI,
    },
    // ch1=L ch2=H ch3=F
    {
        .ODR = ODR_CH1_HI | ODR_CH2_LO | ODR_CH3_LO,
    },
    // ch1=L ch2=F ch3=H
    {
        .ODR = ODR_CH1_HI | ODR_CH2_LO | ODR_CH3_LO,
    },
    // ch1=F ch2=L ch3=H
    {
        .ODR = ODR_CH1_LO | ODR_CH2_HI | ODR_CH3_LO,
    },
    // ch1=H ch2=L ch3=F
    {
        .ODR = ODR_CH1_LO | ODR_CH2_HI | ODR_CH3_LO,
    },
    // ch1=H ch2=F ch3=L
    {
        .ODR = ODR_CH1_LO | ODR_CH2_LO | ODR_CH3_HI,
    },
};
void dev_pwm_set_step(uint8_t step)
{
    CC_REG_STATE cc_state = cc_steps_ccw[step];
    IO_REG_STATE io_state = io_steps_ccw[step];
    disableInterrupts();
    TIM1->CCER1 = cc_state.CCER1;
    TIM1->CCER2 = cc_state.CCER2;
    TIM1->CCMR1 = cc_state.CCMR1;
    TIM1->CCMR2 = cc_state.CCMR2;
    TIM1->CCMR3 = cc_state.CCMR3;
    PORT->ODR = io_state.ODR;
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