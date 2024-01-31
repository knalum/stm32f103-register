#include "adc_drive.h"

/*
PA0 => CH0
PA1 => CH1
PA2 => CH2
PA3 => CH3
PA4 => CH4
PA5 => CH5
PA6 => CH6
PA7 => CH7

PB0 => CH8
PB1 => CH9
*/

char adc_init(char adc, short port, short pin)
{
	char channel;
	char result = 0;
	if (port == PA)
	{
		if (pin < 8)
		{
			result = 1;
			channel = pin;
		}
	}
	else if (port == PB)
	{
		if (pin < 2)
		{
			result = 1;
			channel = 8 + pin;
		}
	}
	else if (port == PC)
	{
		if (pin < 6)
		{
			result = 1;
			channel = 10 + pin;
		}
	}

	if (result)
	{
		init_GP(port, pin, IN, I_AN);
		if (adc == adc1)
		{
			RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_ADC1EN;
			ADC1->CR2 = 0;
			ADC1->SQR3 = channel;
			ADC1->CR2 |= 1;
			DelayMs(100);
			ADC1->CR2 |= 1;
			ADC1->CR2 |= ADC_CR2_ADON | ADC_CR2_CONT;
		}
		else if (adc == adc2)
		{
			RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN | RCC_APB2ENR_ADC2EN);
			ADC2->CR2 = 0;
			ADC2->SQR3 = channel;
			ADC2->CR2 |= 1;
			DelayMs(100);
			ADC2->CR2 |= ADC_CR2_ADON | ADC_CR2_CONT;
		}
	}

	return result;
}

char adc_check(char adc)
{
	char check = 0;
	if (adc == adc1)
	{
		if (ADC1->SR & ADC_SR_EOC)
		{
			// ADC end of conversion
			check = 1;
		}
	}
	else if (adc == adc2)
	{
		if (ADC2->SR & ADC_SR_EOC)
		{
			check = 1;
		}
	}
	return check;
}

// Read ADC value
int adc_rx(char adc, short port, short pin)
{
	int result = 0;
	int data = 0;

	if (adc == adc1)
	{
		data = ADC1->DR;
	}
	else if (adc == adc2)
	{
		data = ADC2->DR;
	}
	// data is between 0-4095 (12 bit ADC)

	// 0xffff = 1111 1111 1111 = 4095
	// result = (data * 1000) / 0xfff;
	result = data;
	return result;
}


void adc_multi_ch_init(char adc, char channels, char * adc_channels)
{
	char i = 0;
	//Initiate the pins
	for(i=0;i< channels;i++)
	{
		if(adc_channels[i]<8)
		{
			init_GP(PA,adc_channels[i],IN,I_AN);
		}
		else if(adc_channels[i]<10)
		{
			init_GP(PB,adc_channels[i]-8,IN,I_AN);
		}
		else if(adc_channels[i]<16)
		{
			init_GP(PC,adc_channels[i]-10,IN,I_AN);
		}
	}
	if(adc == 1)
	{
	//Setup the control registers
	RCC->APB2ENR |= 0x201;
	ADC1->CR2 = 0;
	ADC1->CR2 |= 1;
	DelayMs(100);
	ADC1->SQR3 = adc_channels[1];
	ADC1->CR2 |= 2; //Continous readings
	ADC1->CR2 |= 1; //starting the conversion
	}
	else if(adc == 2) {
	//Setup the control registers
	RCC->APB2ENR |= 0x401;
	ADC2->CR2 = 0;
	ADC2->CR2 |= 1;
	DelayMs(100);
	ADC2->SQR3 = adc_channels[1];
	ADC2->CR2 |= 2; //Continous readings
	ADC2->CR2 |= 1; //starting the conversion
	
	}
}

void adc_multi_ch_rx(char adc, char channels, char * adc_channels, int * analog_rx)
{
	char i = 0;
	int temp_rx = 0;
	if(adc==1)
	{
	while(1)
		{
			if(adc_check(adc1))
			{
				temp_rx = ADC1->DR;
				//analog_rx[i] = (temp_rx  *1000) / 0xFFF;
				analog_rx[i] = temp_rx ;
				i++;
				if(i == channels)
				{
					i = 0;
					ADC1->SQR3 = adc_channels[i];
					break;
				}
				else
				{
					ADC1->SQR3 = adc_channels[i];
				}
			}
			
	}
}
	else if(adc==2)
	{
	while(1)
		{
			if(adc_check(adc1))
			{
				temp_rx = ADC2->DR;
				//analog_rx[i] = (temp_rx  *1000) / 0xFFF;
				analog_rx[i] = temp_rx ;
				i++;
				if(i == channels)
			{
				i = 0;
				ADC2->SQR3 = adc_channels[i];
				break;
			}
			else
			{
				ADC2->SQR3 = adc_channels[i];
			}
			}
			
	}
}
}
