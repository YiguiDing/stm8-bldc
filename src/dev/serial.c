#include "serial.h"
#include "stm8s.h"
void dev_serial_init()
{
    UART1_DeInit();
    UART1_Init(115200, 8, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
    UART1_ITConfig(UART1_IT_RXNE, ENABLE);
    UART1_ITConfig(UART1_IT_IDLE, ENABLE);
    UART1_Cmd(ENABLE);
}
void dev_serial_send(uint8_t *data, uint16_t len)
{
    while (len--)
        while (UART1_GetFlagStatus(UART1_FLAG_TXE))
            UART1_SendData8(*(data++));
}
// __attribute__((weak))
void dev_serial_on_receive(uint8_t *data, uint16_t len)
{
}

#define RX_BUFFER_LENGTH 128
// static uint8_t tx_buf[128], tx_len;
static uint8_t rx_buf[128], rx_len;

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
    if (UART1_GetITStatus(UART1_IT_RXNE))
    {
        rx_buf[rx_len] = UART1_ReceiveData8();
        rx_len = ++rx_len % RX_BUFFER_LENGTH;
        UART1_ClearITPendingBit(UART1_IT_RXNE);
        return;
    }
    else if (UART1_GetFlagStatus(UART1_IT_IDLE))
    {
        dev_serial_on_receive(rx_buf, rx_len);
        rx_len = 0;
        UART1_ClearITPendingBit(UART1_IT_IDLE);
        return;
    }
    else
    {
        return;
    }
}