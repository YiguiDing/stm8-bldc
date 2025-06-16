#include "dev.h"

#include "stdio.h"

uint8_t step = 0;
uint16_t duty = 0;
void task_pwm()
{
  dev_pwm_set_duty(duty += 100);
  dev_pwm_set_step(step++);
  duty %= 1000;
  step %= 6;
  dev_timer_set(Timer_1, 100, task_pwm);
}

void task_led()
{
  dev_led_toggle();
  dev_timer_set(Timer_2, 500, task_led);
}

typedef struct
{
  uint8_t hader;
  uint8_t cmd;
  uint8_t data[4];
} Packet;

typedef union
{
  uint8_t data[6];
  uint8_t packet;
} Command;

void process_command(uint8_t *data, uint16_t len)
{
  dev_serial_send(data, len);
  // Command *command = data;
  // if (command->packet.hader == 0x5A && len >= sizeof Command)
  //   switch (command->packet.cmd)
  //   {
  //   case 0x01:
  //     break;
  //   default:
  //     break;
  //   }
}

typedef union
{
  uint8_t uint8[4];
  int8_t int8[4];
  uint16_t uint16[1];
  int16_t iint16[1];
  uint32_t uint32;
  int32_t int32;
  float f32;
} Data;

typedef struct
{
  Data data[1];
  uint32_t tail;
} JustFloatFrame;
JustFloatFrame tx_frame;

void swap4(uint8_t *p)
{
  uint8_t t;
  t = p[0];
  p[0] = p[3];
  p[3] = t;
  t = p[1];
  p[1] = p[2];
  p[2] = t;
}

void task_serial_tx()
{
  uint8_t idx = 0;
  tx_frame.data[idx].f32 = 0.1234;
  swap4(&tx_frame.data[idx].f32);
  idx++;
  tx_frame.tail = 0x0000807f;
  dev_serial_send((uint8_t *)&tx_frame, sizeof(JustFloatFrame));
  dev_timer_set(Timer_3, 100, task_serial_tx);
}

void main(void)
{

  dev_init();
  dev_timer_set(Timer_1, 1, task_pwm);
  dev_timer_set(Timer_2, 1, task_led);
  // dev_timer_set(Timer_3, 100, task_serial_tx);
  while (1)
    ;
}

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval : None
 */
void assert_failed(u8 *file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
