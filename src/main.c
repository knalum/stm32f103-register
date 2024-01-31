#include "stm32f1xx.h"
#include "systick.h"
#include "gpio_drive.h"
#include "uart_drive.h"

int main(void)
{
  SystemClock_Config_72MHz();
  systick_init();
  init_GP(PC, 13, OUT50, O_GP_PP);

  while (1)
  {
    toggle_GP(PC, 13);
    DelayMs(200);
  }
}
