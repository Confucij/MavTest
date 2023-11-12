#include <platform.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <libopencm3/stm32/gpio.h>
#include <mavlink/standard/mavlink.h>

portTASK_FUNCTION(task_heartbeat, pvParameters) {
    uint32_t *delay = (uint32_t*)pvParameters;
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_toggle(LED_PORT, LED_STATUS);
    }
}

int main() {

    platform_init();

    xTaskCreate(task_heartbeat, "heartbeat", 140, NULL, 0, NULL);

    vTaskStartScheduler();
    for (;;)
        ;
    return 0;
}
