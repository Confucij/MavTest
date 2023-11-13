#ifndef PLATFORM_H
#define PLATFORM_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>


#define LED_STATUS          GPIO13
#define LED_USER            GPIO15
#define LED_PORT            GPIOD

#define COMM_UART           USART2

int platform_init();


#endif //PLATFORM_H