#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f1xx.h"

void systick_init(void);

void SystemClock_Config_72MHz(void);

void delayMs(void);

void DelayMs(unsigned long t);


#endif