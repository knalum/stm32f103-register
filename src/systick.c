
#include "systick.h"

void systick_init(void)
{
  SysTick->CTRL = 0;
  SysTick->LOAD = 0x00FFFFFF;
  SysTick->VAL = 0;
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void delayMs(void)
{
  while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
  {
  }
}

void DelayMillis(void)
{
  SysTick->LOAD = 0x11940;
  SysTick->VAL = 0;
  while ((SysTick->CTRL & 0x00010000) == 0)
    ;
}

void DelayMs(unsigned long t)
{
  for (; t > 0; t--)
  {
    DelayMillis();
  }
}

// Sets up clock to 72MHz
void SystemClock_Config_72MHz(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Configure the main PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9; // For 8 MHz HSE, results in 72 MHz PLL output
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while (1)
      ;
  }

  /* Configure the system clock (HCLK, PCLK1, PCLK2) */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    /* Initialization Error */
    while (1)
      ;
  }
}