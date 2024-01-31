#include "timer_drive.h"

TIM_TypeDef *getTimer(char timer)
{
    TIM_TypeDef *tim = NULL;
    switch (timer)
    {
    case 1:
        tim = TIM1;
        break;
    case 2:
        tim = TIM2;
        break;
    case 3:
        tim = TIM3;
        break;
    case 4:
        tim = TIM4;
        break;
    }
    return tim;
}

TIM_TypeDef *timer_start_micros(char timer, int microS)
{
    int prescaler = 72 - 1;
    TIM_TypeDef *tim;

    if (timer == 1)
    {
        // RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
        RCC->APB2ENR |= 0x0800;
    }
    else
    {
        RCC->APB1ENR |= 1 << (timer - 2);
    }
    tim = getTimer(timer);

    tim->CNT = 0;            // Start counter value
    tim->PSC = prescaler;    // Prescaler : 1100 counts per second
    tim->ARR = (microS - 3); // Auto reload register
    tim->CR1 |= 0x1;         // Timer enable (One puls mode. Stop counting after first)
    return tim;
}

TIM_TypeDef *timer_start_millis(char timer, int millis)
{
    int prescaler = (36000 - 1);
    TIM_TypeDef *tim;

    if (timer == 1)
    {
        //        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
        RCC->APB2ENR |= 0x0800;
    }
    else
    {
        RCC->APB1ENR |= 1 << (timer - 2);
    }
    tim = getTimer(timer);

    tim->CNT = 0;            // Start counter value
    tim->PSC = prescaler;    // Prescaler : 1100 counts per second
    tim->ARR = (millis * 2); // Auto reload register
    tim->CR1 |= 0x1;         // Timer enable (One puls mode. Stop counting after first)
    return tim;
}

void TDelay_Micros(char timer, int microS)
{
    TIM_TypeDef *tim = timer_start_micros(timer, microS);
    tim->CR1 |= 0x9;
    while (tim->CR1 & 1)
    {
    }
}

void TDelay_Millis(char timer, int millis)
{
    for (int i = 0; i < millis; i++)
    {
        TDelay_Micros(timer, 1000);
    }
}

void timer_irq_micro_start(char timer, int microS)
{
    TIM_TypeDef *tim = timer_start_micros(timer, microS);
    tim->DIER |= 1;

    __disable_irq();
    switch (timer)
    {
    case 1:
        NVIC_EnableIRQ(TIM1_UP_IRQn);
        break;
    case 2:
        NVIC_EnableIRQ(TIM2_IRQn);
        break;
    case 3:
        NVIC_EnableIRQ(TIM3_IRQn);
        break;
    case 4:
        NVIC_EnableIRQ(TIM4_IRQn);
        break;
    }
    __enable_irq();
}

void timer_irq_millis_start(char timer, int millis)
{
    TIM_TypeDef *tim = timer_start_millis(timer, millis);
    tim->DIER |= 1;

    __disable_irq();
    switch (timer)
    {
    case 1:
        NVIC_EnableIRQ(TIM1_UP_IRQn);
        break;
    case 2:
        NVIC_EnableIRQ(TIM2_IRQn);
        break;
    case 3:
        NVIC_EnableIRQ(TIM3_IRQn);
        break;
    case 4:
        NVIC_EnableIRQ(TIM4_IRQn);
        break;
    }
    __enable_irq();
}

void timer_irq_Rflag(char timer)
{
    TIM_TypeDef *tim = getTimer(timer);
    tim->SR &= ~TIM_SR_UIF; // Clear the timer interrupt flag
}

void timer_irq_stop(char timer)
{
    TIM_TypeDef *tim = getTimer(timer);
    tim->DIER &= ~TIM_DIER_UIE; // Update interrupt enable (disable it)

    __disable_irq();
    switch (timer)
    {
    case 1:
        NVIC_DisableIRQ(TIM1_UP_IRQn);
        break;
    case 2:
        NVIC_DisableIRQ(TIM2_IRQn);
        break;
    case 3:
        NVIC_DisableIRQ(TIM3_IRQn);
        break;
    case 4:
        NVIC_DisableIRQ(TIM4_IRQn);
        break;
    }
    __enable_irq();
}

void timer_stop(char timer)
{
    TIM_TypeDef *tim = getTimer(timer);
    tim->CR1 = 0;

    if (timer == 1)
    {
        RCC->APB2ENR &= ~0x0800;
    }
    else
    {
        RCC->APB1ENR &= ~(1 << (timer - 2));
    }

    timer_irq_stop(timer);
}

char get_channel(char port, char pin)
{
    if (port == 1)
    {
        switch (pin)
        {
        case 0:
            return 1;
        case 1:
            return 2;
        case 2:
            return 3;
        case 3:
            return 4;

        case 6:
            return 1;
        case 7:
            return 2;

        case 8:
            return 1;
        case 9:
            return 2;
        case 10:
            return 3;
        case 11:
            return 4;
        }
        return 0;
    }
    else if (port == 2)
    {
        switch (pin)
        {
        case 0:
            return 3;
        case 1:
            return 4;
        }
        return 0;
    }
    return 0;
}

char get_timer_pin(char port, char pin)
{
    if (port == 1)
    {
        if (pin < 4)
        {
            return 2;
        }
        else if (pin > 7 && pin < 12)
        {
            return 1;
        }
        else if (pin > 5 && pin < 8)
        {
            return 3;
        }

        return -1;
    }
    else if (port == 2)
    {
        if (pin < 2)
        {
            return 3;
        }
        return -1;
    }
    return -1;
}

TIM_TypeDef *get_timer_address(char port, char pin)
{
    if (port == 1)
    {
        if (pin < 4)
        {
            return TIM2;
        }
        else if (pin > 7 && pin < 12)
        {
            return TIM1;
        }
        else if (pin > 5 && pin < 8)
        {
            return TIM3;
        }

        return 0;
    }
    else if (port == 2)
    {
        if (pin < 2)
        {
            return TIM3;
        }
        return 0;
    }
    return 0;
}

void timer_compare_micro(char port, char pin, int arr, int compare)
{
    TIM_TypeDef *tim = get_timer_address(port, pin);
    char timer = get_timer_pin(port, pin);
    char channel = get_channel(port, pin);

    if ((tim->CCER & (1 << ((channel - 1) * 4))) == 0)
    {
        if (timer == 1)
        {
            RCC->APB2ENR |= 0x0800;
        }

        else
        {
            RCC->APB1ENR |= 1 << (timer - 2);
        }
        init_GP(port, pin, OUT50, O_AF_PP);
        tim->PSC = (72 - 1);
        switch (channel)
        {
        case 1:
            tim->CCMR1 |= 0x30;
            break;
        case 2:
            tim->CCMR1 |= 0x3000;
            break;
        case 3:
            tim->CCMR2 |= 0x30;
            break;
        case 4:
            tim->CCMR2 |= 0x3000;
            break;
        }

        tim->CCER |= (1 << ((channel - 1) * 4));
        tim->BDTR |= 0x8000;
        tim->CR1 |= 1;
    }
    tim->ARR = (arr - 1);

    switch (channel)
    {
    case 1:
        tim->CCR1 = compare;
        break;
    case 2:
        tim->CCR2 = compare;
        break;
    case 3:
        tim->CCR3 = compare;
        break;
    case 4:
        tim->CCR4 = compare;
        break;
    }
}

void timer_compare_milli(char port, char pin, int arr, int compare)
{
    TIM_TypeDef *tim = get_timer_address(port, pin);
    char timer = get_timer_pin(port, pin);
    char channel = get_channel(port, pin);

    if ((tim->CCER & (1 << ((channel - 1) * 4))) == 0)
    {
        if (timer == 1)
        {
            RCC->APB2ENR |= 0x0800;
        }

        else
        {
            RCC->APB1ENR |= 1 << (timer - 2);
        }
        init_GP(port, pin, OUT50, O_AF_PP);
        tim->PSC = (36000 - 1);
        switch (channel)
        {
        case 1:
            tim->CCMR1 |= 0x30;
            break;
        case 2:
            tim->CCMR1 |= 0x3000;
            break;
        case 3:
            tim->CCMR2 |= 0x30;
            break;
        case 4:
            tim->CCMR2 |= 0x3000;
            break;
        }

        tim->CCER |= (1 << ((channel - 1) * 4));
        tim->BDTR |= 0x8000;
        tim->CR1 |= 1;
    }
    tim->ARR = (arr * 2 - 1);

    compare *= 2;

    switch (channel)
    {
    case 1:
        tim->CCR1 = compare;
        break;
    case 2:
        tim->CCR2 = compare;
        break;
    case 3:
        tim->CCR3 = compare;
        break;
    case 4:
        tim->CCR4 = compare;
        break;
    }
}

void timer_pwm_milli(char port, char pin, int arr, int compare)
{
    TIM_TypeDef *tim = get_timer_address(port, pin);
    char timer = get_timer_pin(port, pin);
    char channel = get_channel(port, pin);

    if ((tim->CCER & (1 << ((channel - 1) * 4))) == 0)
    {
        if (timer == 1)
        {
            RCC->APB2ENR |= 0x0800;
        }

        else
        {
            RCC->APB1ENR |= 1 << (timer - 2);
        }
        init_GP(port, pin, OUT50, O_AF_PP);
        tim->PSC = (36000 - 1);
        switch (channel)
        {
        case 1:
            tim->CCMR1 |= 0x60;
            break;
        case 2:
            tim->CCMR1 |= 0x6000;
            break;
        case 3:
            tim->CCMR2 |= 0x60;
            break;
        case 4:
            tim->CCMR2 |= 0x6000;
            break;
        }

        tim->CCER |= (1 << ((channel - 1) * 4));
        tim->BDTR |= 0x8000;
        tim->CR1 |= 1;
    }
    tim->ARR = (arr * 2 - 1);

    compare *= 2;

    switch (channel)
    {
    case 1:
        tim->CCR1 = compare;
        break;
    case 2:
        tim->CCR2 = compare;
        break;
    case 3:
        tim->CCR3 = compare;
        break;
    case 4:
        tim->CCR4 = compare;
        break;
    }
}

void timer_pwm_micro(char port, char pin, int arr, int compare)
{
    TIM_TypeDef *tim = get_timer_address(port, pin);
    char timer = get_timer_pin(port, pin);
    char channel = get_channel(port, pin);

    if ((tim->CCER & (1 << ((channel - 1) * 4))) == 0)
    {
        if (timer == 1)
        {
            // Setup the Counter at the bus level
            RCC->APB2ENR |= 0x0800;
        }
        else
        {
            RCC->APB1ENR |= 1 << (timer - 2);
        }
        init_GP(port, pin, OUT50, O_AF_PP);
        tim->PSC = (72 - 1);
        switch (channel)
        {
        case 1:
            tim->CCMR1 |= 0x60;
            break;
        case 2:
            tim->CCMR1 |= 0x6000;
            break;
        case 3:
            tim->CCMR2 |= 0x60;
            break;
        case 4:
            tim->CCMR2 |= 0x6000;
            break;
        }
        tim->CCER |= (1 << ((channel - 1) * 4));
        tim->BDTR |= 0x8000;
        tim->CR1 |= 1;
    }
    tim->ARR = (arr - 1);
    switch (channel)
    {
    case 1:
        tim->CCR1 = compare;
        break;
    case 2:
        tim->CCR2 = compare;
        break;
    case 3:
        tim->CCR3 = compare;
        break;
    case 4:
        tim->CCR4 = compare;
        break;
    }
}

char servo_180_deg(char port, char pin, int deg)
{
    if (deg > 180)
    {
        return 0;
    }
    int deg_pwm = 400 + deg * 11;
    timer_pwm_micro(port, pin, 21000, deg_pwm);
    return 1;
}

void timer_capture_micro(char port, char pin, int arr, int EDGE)
{
    TIM_TypeDef *tim = get_timer_address(port, pin);
    char timer = get_timer_pin(port, pin);
    char channel = get_channel(port, pin);

    if ((tim->CCER & (1 << ((channel - 1) * 4))) == 0)
    {
        if (timer == 1)
        {
            RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
        }

        else
        {
            RCC->APB1ENR |= 1 << (timer - 2);
        }
        init_GP(port, pin, IN, I_PP);
        tim->PSC = (72 - 1);
        switch (channel)
        {
        case 1:
            tim->CCMR1 |= 0x1;
            break;
        case 2:
            tim->CCMR1 |= 0x100;
            break;
        case 3:
            tim->CCMR2 |= 0x1;
            break;
        case 4:
            tim->CCMR2 |= 0x100;
            break;
        }

        tim->CCER |= (1 << ((channel - 1) * 4));
        tim->BDTR |= 0x8000;
        tim->CR1 |= 1;
    }

    if (EDGE)
    {
        tim->CCER |= (1 << (((channel - 1) * 4) + 1));
    }
    else
    {
        tim->CCER &= ~(1 << (((channel - 1) * 4) + 1));
    }
    tim->ARR = (arr - 1);
}

int timer_get_capture_micro(char port, char pin)
{
    TIM_TypeDef *tim = get_timer_address(port, pin);
    int my_capture = 0;
    int my_psc = 0;
    char channel = get_channel(port, pin);

    switch (channel)
    {
    case 1:
        my_capture = tim->CCR1;
        break;
    case 2:
        my_capture = tim->CCR2;
        break;
    case 3:
        my_capture = tim->CCR3;
        break;
    case 4:
        my_capture = tim->CCR4;
        break;
    }

    my_psc = tim->PSC;
    my_capture = (my_capture * my_psc) / (72 - 1);

    return my_capture;
}

int timer_get_capture_milli(char port, char pin)
{
    int my_capture = timer_get_capture_micro(port, pin) / 1000;
    return my_capture;
}

char timer_channel_stop(char port, char pin)
{
    TIM_TypeDef *tim = get_timer_address(port, pin);
    char channel = get_channel(port, pin);
    tim->CCER &= ~(1 << ((channel - 1) * 4));

    switch(channel){
        case 1: tim->CCR1 = 0;break;
        case 2: tim->CCR2 = 0;break;
        case 3: tim->CCR3 = 0;break;
        case 4: tim->CCR4 = 0;break;
    }
}