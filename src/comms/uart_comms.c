#include "comms.h"
#include <stdio.h>
#include <string.h>
#include "driver/uart.h"

#define UART_PORT    UART_NUM_0
#define UART_BAUD    115200
#define UART_TX_PIN  1
#define UART_RX_PIN  3
#define BUF_SIZE     1024

/**
 * @brief Initialize UART communication
 * To switch protocol (e.g. MQTT, BLE):
 * - Create new file e.g. mqtt_comms.c
 * - Implement comms_init() and comms_send()
 * - Replace uart_comms.c in build
 */
void comms_init(void) {
    uart_config_t uart_config = {
        .baud_rate  = UART_BAUD,
        .data_bits  = UART_DATA_8_BITS,
        .parity     = UART_PARITY_DISABLE,
        .stop_bits  = UART_STOP_BITS_1,
        .flow_ctrl  = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, UART_TX_PIN, UART_RX_PIN,
                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_PORT, BUF_SIZE, 0, 0, NULL, 0);
    printf("UART initialized at %d baud\n", UART_BAUD);
}

/**
 * @brief Send message over UART
 * @param message Null-terminated string
 */
void comms_send(const char *message) {
    uart_write_bytes(UART_PORT, message, strlen(message));
}