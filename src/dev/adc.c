#include "adc.h"
#include "stm8s.h"

void dev_adc_init()
{
    GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);
    ADC1_DeInit();
    ADC1_ConversionConfig(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_0, ADC1_ALIGN_LEFT);
    ADC1_PrescalerConfig(ADC1_PRESSEL_FCPU_D2);
    ADC1_ExternalTriggerConfig(ADC1_EXTTRIG_TIM, ENABLE);
    ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL0 | ADC1_SCHMITTTRIG_CHANNEL1, ENABLE);
    ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);
    ADC1_Cmd(ENABLE);
}

void dev_adc_on_update()
{
}

INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{
    ADC1_ClearITPendingBit(ADC1_IT_EOC);
    dev_adc_on_update();
}