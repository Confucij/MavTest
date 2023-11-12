#include <FreeRTOS.h>
#include <task.h>

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    for (;;) {
    }
}

void vApplicationIdleHook(void) {}

void vApplicationMallocFailedHook(void) {}

void mem_manage_handler() {
    while (1)
        ;
}

void bus_fault_handler() {
    while (1)
        ;
}
void usage_fault_handler() {
    while (1)
        ;
}
void debug_monitor_handler() {
    while (1)
        ;
}

void hard_fault_handler() {
    while (1)
        ;
}