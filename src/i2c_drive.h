#include "stm32f1xx.h"

#define i2c_FM 0x2d
#define i2c_SM 0xB4

void i2c_init(char i2c,unsigned short speed_mode);

void i2c_write(char i2c,char address,char data[],int len);