#include "stm32f1xx.h"
#include "gpio_drive.h"

void init_GP(unsigned short port, unsigned short pin, unsigned short dir, unsigned short opt)
{
    volatile unsigned long *CR;
    unsigned short tPIN = pin;
    unsigned short offset = 0x00;

    if (pin > 7)
    {
        tPIN -= 8;
        offset = 0x01;
    }

    if (port == PA)
    {
        RCC_APB2ENR |= 4; // Enable Port A
        CR = (volatile unsigned long *)(&GPIO_A + offset);
    }
    else if (port == PB)
    {
        RCC_APB2ENR |= 8; // Enable Port B
        CR = (volatile unsigned long *)(&GPIO_B + offset);
    }
    else if (port == PC)
    {
        RCC_APB2ENR |= 0x10; // Enable Port C
        CR = (volatile unsigned long *)(&GPIO_C + offset);
    }

    *CR &= ~(0xF << (tPIN) * 4);                            // Reset target pin  // 0xFFFFFFF0
    *CR |= ((dir << (tPIN * 4)) | (opt << (tPIN * 4 + 2))); // Set up direction and option
}

void W_GP(unsigned short port, unsigned short pin, unsigned short state)
{
    volatile unsigned long *ODR;
    unsigned long offset = 0x03;

    if (port == PA)
    {
        ODR = (volatile unsigned long *)(&GPIO_A + offset);
    }
    else if (port == PB)
    {
        ODR = (volatile unsigned long *)(&GPIO_B + offset);
    }
    else if (port == PC)
    {
        ODR = (volatile unsigned long *)(&GPIO_C + offset);
    }
    state ? (*ODR |= (state << pin)) : (*ODR &= ~(1 << pin));
}

int R_GP(unsigned short port, unsigned short pin)
{
    volatile unsigned long *IDR;
    unsigned long offset = 0x02;

    int state;
    if (port == PA)
    {
        IDR = (volatile unsigned long *)(&GPIO_A + offset);
    }
    else if (port == PB)
    {
        IDR = (volatile unsigned long *)(&GPIO_B + offset);
    }
    else if (port == PC)
    {
        IDR = (volatile unsigned long *)(&GPIO_C + offset);
    }
    state = ((*IDR & (1 << pin))) >> pin;
    return state;
}

void toggle_GP(unsigned short port, unsigned short pin)
{
    if (R_GP(port, pin) == LOW)
    {
        W_GP(port, pin, HIGH);
    }
    else
    {
        W_GP(port, pin, LOW);
    }
}