#include <platform.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <memory.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <mavlink/common/mavlink.h>
#include "system_config.h"

#define QUEUE_LEN 16
#define HEARTBEET_PERIOD 1000
#define HEARTBEET_BLINK_PERIOD 150

QueueHandle_t incoming_queue;
QueueHandle_t outgoing_queue;
QueueSetHandle_t comm_set;

portTASK_FUNCTION(task_heartbeat, pvParameters) {
    uint32_t *delay = (uint32_t *)pvParameters;
    float led_state = 1;
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(HEARTBEET_PERIOD - HEARTBEET_BLINK_PERIOD));

        // TODO: Send your data here
        mavlink_message_t *heartbeat_message = pvPortMalloc(sizeof(mavlink_message_t));
        if (heartbeat_message == NULL) {
            continue;
        }
        mavlink_msg_heartbeat_pack(SYSTEM_ID, MAV_COMP_ID_USER1, heartbeat_message, MAV_TYPE_GENERIC,
                                   MAV_AUTOPILOT_INVALID, MAV_MODE_FLAG_TEST_ENABLED, 0, 0);

        if (pdTRUE != xQueueSend(outgoing_queue, (void *)&heartbeat_message, 0)) {
            vPortFree(heartbeat_message);
        }

        gpio_set(LED_PORT, LED_STATUS);
        vTaskDelay(pdMS_TO_TICKS(HEARTBEET_BLINK_PERIOD));
        gpio_clear(LED_PORT, LED_STATUS);
    }
}

portTASK_FUNCTION(task_communicator, pvParameters) {
    mavlink_status_t receive_status;
    uint8_t send_buffer[MAVLINK_MAX_PACKET_LEN] = {0};

    for (;;) {
        uint8_t symbol = 0;
        QueueSetMemberHandle_t event_queue = xQueueSelectFromSet(comm_set, portMAX_DELAY);
        if (event_queue == incoming_queue) {
            if (xQueueReceive(incoming_queue, &symbol, 0)) {
                mavlink_message_t incomming_message;
                if (mavlink_parse_char(MAVLINK_COMM_0, symbol, &incomming_message, &receive_status)) {
                    switch (incomming_message.msgid) {
                        case MAVLINK_MSG_ID_COMMAND_LONG: {
                            mavlink_command_long_t command;
                            mavlink_msg_command_long_decode(&incomming_message, &command);
                            if (command.target_system != SYSTEM_ID && command.target_component != MAV_COMP_ID_USER1) {
                                continue;
                            }
                            switch (command.command) {
                                case MAV_CMD_USER_1: {
                                    gpio_toggle(LED_PORT, LED_USER);
                                    break;
                                }
                                default: {
                                    break;
                                }
                            }
                            break;
                        }
                        default: {
                            break;
                        }
                    }
                }
            }
        } else if (event_queue == outgoing_queue) {
            mavlink_message_t *message;
            if (xQueueReceive(outgoing_queue, &message, 0)) {
                uint16_t content_len = mavlink_msg_to_send_buffer(send_buffer, message);
                // TODO: this isn't a most efficient way to send data. Think of using DMA
                // Full message may take up to 30ms to transmit which is acceptable for our application
                for (uint16_t idx = 0; idx < content_len; idx++) {
                    usart_send_blocking(COMM_UART, send_buffer[idx]);
                }

                vPortFree(message);
            }
        }
    }
}

int main() {

    platform_init();

    xTaskCreate(task_heartbeat, "heartbeat", 140, (void *)1000, 0, NULL);
    xTaskCreate(task_communicator, "comm", 500, NULL, 0, NULL);

    incoming_queue = xQueueCreate(QUEUE_LEN, sizeof(uint8_t));
    outgoing_queue = xQueueCreate(QUEUE_LEN, sizeof(void *));
    comm_set = xQueueCreateSet(16);
    xQueueAddToSet(incoming_queue, comm_set);
    xQueueAddToSet(outgoing_queue, comm_set);

    vTaskStartScheduler();
    for (;;)
        ;
    return 0;
}
