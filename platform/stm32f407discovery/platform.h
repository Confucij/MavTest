#ifndef PLATFORM_H
#define PLATFORM_H

#include <libopencm3/stm32/gpio.h>


#define LED_STATUS          GPIO13
#define LED_PORT            GPIOD


//TODO: Docs
int platform_init();


#endif //PLATFORM_H