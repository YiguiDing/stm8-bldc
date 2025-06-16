#include "adc.h"
#include "stm8s.h"

/**
 * f_Master = 16Mhz
 * f_ADC = f_Master/2 = 8Mhz
 * f_conv = f_ADC/14 = 0.571428 Mhz = 571.428 khz
 * t_conv = 1/f_conv = 1.75us = 0.00175 ms
 * 四通道: t_conv4 = t_conv * 4 = 7us
 * 四通道: f_conv4 = 1/ t_conv4 = 1 / 7 =  0.142857Mhz = 142.857khz
 */
void dev_adc_init()
{
    // PD2(AIN3)
    // PD3(AIN4)
    // PD5(AIN5)
    // PD6(AIN6)
    GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT); // 上拉
    GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT); // 上拉
    GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT); // 上拉
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT); // 上拉

    ADC1_DeInit();
    ADC1_ExternalTriggerConfig(ADC1_EXTTRIG_TIM, ENABLE);          // 定时器触发
    ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D2);                    // 2分频
    ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL4, DISABLE); // 禁用施密特触发器
    ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE,              // 单次转换
                          ADC1_CHANNEL_3,                          // 0~3 四个通道
                          ADC1_ALIGN_LEFT                          // 左对齐
    );
    ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE); // 转换完成中断
    ADC1_Cmd(ENABLE);                     // 启用
}

void dev_adc_on_complete()
{
}

INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{
    ADC1_ClearITPendingBit(ADC1_IT_EOC);
    dev_adc_on_complete();
}