#include <platform.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <stdint.h>

int main() {

    platform_init();
    for (;;) {
        for (int i = 0; i < 8000000; i++) {
            __asm__("nop");
        }
        gpio_toggle(LED_PORT, LED_STATUS);
    }

    return 0;
}
