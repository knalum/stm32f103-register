#include "watchdog_drive.h"

void watchdogInit(){
    // Must reset watchdog timer in loop

  RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;
  WWDG->CFR |= (0b10000000) | 0x64;
  WWDG->CR |= WWDG_CR_WDGA;
}
