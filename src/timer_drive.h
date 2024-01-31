#ifndef TIMER_DRIVE_H
#define TIMER_DRIVE_H

#include "stm32f1xx.h"
#include "gpio_drive.h"
enum timers
{
    T,
    T1,
    T2,
    T3,
    T4
};

void TDelay_Micros(char timer, int microS);
void TDelay_Millis(char timer, int millis);
void TDelay_Millis(char timer, int milliS);

void timer_irq_micro_start(char timer, int microS);
void timer_irq_millis_start(char timer, int millis);
void timer_irq_Rflag(char timer);

void timer_stop(char timer);
void timer_irq_stop(char timer);

char get_channel(char port, char pin);
char get_timer_pin(char port, char pin);
TIM_TypeDef *get_timer_address(char port, char pin);
void timer_compare_micro(char port, char pin, int arr, int compare);
void timer_compare_milli(char port, char pin, int arr, int compare);
void timer_pwm_milli(char port, char pin, int arr, int compare);
void timer_pwm_micro(char port, char pin, int arr, int compare);
char servo_180_deg(char port, char pin, int deg);

int timer_get_capture_milli(char port, char pin);
int timer_get_capture_micro(char port, char pin);
char timer_channel_stop(char port, char pin);
#endif