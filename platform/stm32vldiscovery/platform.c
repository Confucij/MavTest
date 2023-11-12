#include "platform.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/scb.h>
#include <stdint.h>

int platform_init() {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_24MHZ]);
    rcc_periph_clock_enable(RCC_GPIOC);
    scb_set_priority_grouping(SCB_AIRCR_PRIGROUP_GROUP16_NOSUB);
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_STATUS);
    return 0;
}


extern void vPortSVCHandler( void ) __attribute__ (( naked ));
extern void xPortPendSVHandler( void ) __attribute__ (( naked ));
extern void xPortSysTickHandler( void );

void sv_call_handler(void) {
	vPortSVCHandler();
}

void pend_sv_handler(void) {
	xPortPendSVHandler();
}

void sys_tick_handler(void) {
	xPortSysTickHandler();
}