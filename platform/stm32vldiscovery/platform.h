#ifndef PLATFORM_H
#define PLATFORM_H

#include <libopencm3/stm32/gpio.h>

#define LED_STATUS GPIO8
#define LED_PORT GPIOC

// TODO: Docs
int platform_init();

#endif // PLATFORM_H