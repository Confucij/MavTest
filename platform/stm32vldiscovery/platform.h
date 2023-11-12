#ifndef PLATFORM_H
#define PLATFORM_H

#include <libopencm3/stm32/gpio.h>

#define LED_STATUS  GPIO9
#define LED_USER    GPIO8
#define LED_PORT    GPIOC

#define COMM_UART   USART2

int platform_init();

#endif // PLATFORM_H