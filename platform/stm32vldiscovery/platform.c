#include "platform.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/f1/nvic.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/scb.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <stdint.h>

extern QueueHandle_t incoming_queue;

int platform_init() {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_24MHZ]);
    // Required for FreeRTOS to function
    scb_set_priority_grouping(SCB_AIRCR_PRIGROUP_GROUP16_NOSUB);

    // Enable LED peripheral
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_STATUS | LED_USER);

    // Setup USART peripheral
    rcc_periph_clock_enable(RCC_USART2);
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO2);
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO3);
    usart_set_baudrate(COMM_UART, 115200);
    usart_set_databits(COMM_UART, 8);
    usart_set_stopbits(COMM_UART, USART_STOPBITS_1);
    usart_set_mode(COMM_UART, USART_MODE_TX_RX);
    usart_set_parity(COMM_UART, USART_PARITY_NONE);
    usart_set_flow_control(COMM_UART, USART_FLOWCONTROL_NONE);
    usart_enable_rx_interrupt(COMM_UART);
    nvic_enable_irq(NVIC_USART2_IRQ);
    nvic_set_priority(NVIC_USART2_IRQ, configMAX_SYSCALL_INTERRUPT_PRIORITY + 1); // Priority for ISR that uses FreeRTOS API should be lower that of syscall
    usart_enable(COMM_UART);

    return 0;
}

void usart2_isr(void) {
    /* Check if we were called because of RXNE. */
    if (((USART_CR1(COMM_UART) & USART_CR1_RXNEIE) != 0) && ((USART_SR(COMM_UART) & USART_SR_RXNE) != 0)) {
        /* Retrieve the data from the peripheral. */
        uint8_t data = usart_recv(COMM_UART);
        xQueueSendFromISR(incoming_queue, &data, NULL);
    }
}