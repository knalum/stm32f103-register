#include "uart_drive.h"

// Must implement void USART3_IRQHandler() to handle interrupt
void UART_init(unsigned short usart, unsigned long BR)
{
    unsigned long BRR_Cal;
    BRR_Cal = USART_BRR_Custom(usart, BR);
    RCC->APB2ENR |= 1; // Enable alternative function for pins

    __disable_irq();
    if (usart == 1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Enable UART
        init_GP(PA, 9, OUT50, O_AF_PP);       // TX
        init_GP(PA, 10, IN, I_PP);            // RX

        USART1->BRR = BRR_Cal;
        USART1->CR1 |= USART_CR1_TE; // Transmit
        USART1->CR1 |= USART_CR1_RE; // Receive
        USART1->CR1 |= USART_CR1_UE; // Enable USART

        USART1->CR1 |= USART_CR1_RXNEIE;
        NVIC_EnableIRQ(USART1_IRQn);
    }
    else if (usart == 2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Enable UART2
        init_GP(PA, 2, OUT50, O_AF_PP);       // PA2 TX
        init_GP(PA, 2, OUT50, O_AF_PP);       // PA2 TX
        init_GP(PA, 3, IN, I_PP);             // PA3 RX

        USART2->BRR = BRR_Cal;
        USART2->CR1 |= 8;      // Transmit
        USART2->CR1 |= 4;      // Receive
        USART2->CR1 |= 0x2000; // Enable USART: 0010 0000 0000 0000

        USART2->CR1 |= USART_CR1_RXNEIE;
        NVIC_EnableIRQ(USART2_IRQn);
    }
    else if (usart == 3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // Enable UART
        init_GP(PB, 10, OUT50, O_AF_PP);      //  TX
        init_GP(PB, 11, IN, I_PP);            // PA3 RX

        USART3->BRR = BRR_Cal;
        USART3->CR1 |= 8;      // Transmit
        USART3->CR1 |= 4;      // Receive
        USART3->CR1 |= 0x2000; // Enable USART

        USART3->CR1 |= USART_CR1_RXNEIE;
        NVIC_EnableIRQ(USART3_IRQn);
    }
    __enable_irq();
}
unsigned long USART_BRR_Custom(unsigned short usart, unsigned long BR)
{
    unsigned long div = 36000000UL;
    unsigned long dec;
    unsigned long final;
    double frac = 36000000.00;
    double frac2 = 1.00;

    if (usart == 1)
    {
        div = 72000000UL;
        frac = 72000000.00;
    }
    div = div / (BR * 16);
    frac = 16 * ((frac / (BR * 16)) - div);
    dec = frac;
    frac2 = 100 * (frac - dec);
    if (frac2 > 50)
    {
        dec++;
        if (dec == 16)
        {
            dec = 0;
            div++;
        }
    }

    final = (div << 4);
    final += dec;

    return final;
}

void transmitChar(char ch, USART_TypeDef *usart)
{
    while ((usart->SR & USART_SR_TXE) == 0)
    {
        __NOP();
    }
    usart->DR = ch;
}

void transmitString(char str[], USART_TypeDef *usart)
{
    int i = 0;
    while (str[i] != '\0')
    {
        transmitChar(str[i], usart);
        i++;
        if (i > 100)
        {
            // Safety...
            break;
        }
    }
}

char receiveChar(USART_TypeDef *uart)
{
    char c;
    while ((uart->SR & 0x20) == 0x00)
    {
    };
    c = uart->DR;

    return c;
}

void receiveString(char out[], USART_TypeDef *usart)
{
    int i = 0;

    char in = '\0';
    while ((in = receiveChar(usart)) != '\n')
    {
        out[i] = in;
        i++;
    }
}

void UART_ISR(USART_TypeDef *uart, unsigned short bridge, unsigned short *signal, unsigned short *counter, char str[])
{
    if (bridge == 0)
    {
        str[*counter] = receiveChar(uart);
        if (str[*counter] == '\n')
        {
            *counter = 0;
            *signal = 1;
        }
        else
        {
            *counter++;
        }
    }
    else
    {
        // Skip bridge...
    }
}