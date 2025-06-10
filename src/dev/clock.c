#include "clock.h"
#include "stm8s.h"
/**
 * 使用内部16Mhz时钟
 * f_Master=16Mhz
 * f_CPU=16Mhz
 * f_Peripheral=16Mhz
 * 开启TIM1、UART1、ADC时钟
 */
void dev_clock_init()
{
    CLK_DeInit();
    CLK_HSICmd(ENABLE);                                                                                   // 开启内部时钟(默认开)
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);                                                        // HSI 1分频(默认8)
    return;
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);                                                              // CPU 1分频(默认1)
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_MANUAL, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE); // 选择内部时钟(默认内部时钟)
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);                                             // 开启定时器1时钟(默认开)
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);                                              // 开启串口1时钟(默认开)
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);                                                // 开启ADC时钟(默认开)
}