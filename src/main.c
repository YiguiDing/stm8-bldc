#include "dev.h"

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
void main(void)
{

  dev_init();
  dev_timer_set(Timer_1, 1, task_pwm);
  dev_timer_set(Timer_2, 1, task_led);
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
