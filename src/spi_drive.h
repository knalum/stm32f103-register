#include "stm32f1xx.h"
#include "gpio_drive.h"

void spi_init(unsigned short spi);

void spi_tx(unsigned short spi, char tx_char);

void spi_msg(unsigned short spi, char str[]);