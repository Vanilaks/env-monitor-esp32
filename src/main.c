#include <stdio.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "driver/gpio.h"

#include "sensor/sensor.h"
#include "config/config.h"
#include "comms/comms.h"

#define BUTTON_PIN GPIO_NUM_13
#define LED_PIN    GPIO_NUM_2

typedef enum {
    MODE_CONFIG,
    MODE_WORKING
} device_mode_t;

static device_mode_t current_mode = MODE_CONFIG;

/* =========================================================
   BUTTON TASK
   ========================================================= */
static void button_task(void *arg) {

    gpio_config_t btn_cfg = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&btn_cfg);

    int last_state = 1;

    while (1) {

        int state = gpio_get_level(BUTTON_PIN);

        // Detect button press
        if (last_state == 1 && state == 0) {

            if (current_mode == MODE_CONFIG) {

                printf("Switching to WORKING MODE\n");
                current_mode = MODE_WORKING;

            } else {

                printf("Switching to CONFIG MODE\n");
                current_mode = MODE_CONFIG;
            }

            // Debounce delay
            vTaskDelay(pdMS_TO_TICKS(300));
        }

        last_state = state;

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/* =========================================================
   LED TASK
   ========================================================= */
static void led_task(void *arg) {

    gpio_config_t led_cfg = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&led_cfg);

    int led_state = 0;

    while (1) {

        if (current_mode == MODE_CONFIG) {

            // Solid ON in config mode
            gpio_set_level(LED_PIN, 1);

        } else {

            // Blink in working mode
            led_state = !led_state;
            gpio_set_level(LED_PIN, led_state);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/* =========================================================
   MONITOR TASK
   ========================================================= */
static void monitor_task(void *arg) {

    config_t cfg;

    config_load(&cfg);

    while (1) {

        if (current_mode == MODE_WORKING) {

            float temp = sensor_read_temperature();

            if (temp == SENSOR_ERROR) {

                comms_send("ERROR: Sensor read failed\n");

            } else {

                char msg[64];

                if (temp > cfg.temp_high) {

                    snprintf(
                        msg,
                        sizeof(msg),
                        "ALERT HIGH: %.2f C\n",
                        temp
                    );

                } else if (temp < cfg.temp_low) {

                    snprintf(
                        msg,
                        sizeof(msg),
                        "ALERT LOW: %.2f C\n",
                        temp
                    );

                } else {

                    snprintf(
                        msg,
                        sizeof(msg),
                        "TEMP: %.2f C\n",
                        temp
                    );
                }

                comms_send(msg);
            }

            vTaskDelay(
                pdMS_TO_TICKS(cfg.report_interval_ms)
            );

        } else {

            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

/* =========================================================
   MAIN
   ========================================================= */
void app_main(void) {

    printf("\n");
    printf("=================================\n");
    printf(" ENVIRONMENT MONITOR STARTING\n");
    printf("=================================\n");

    // Initialize modules
    sensor_init();
    comms_init();

    // Default configuration
    config_t cfg = {
        .report_interval_ms = 5000,
        .temp_high = 35.0f,
        .temp_low = 10.0f
    };

    config_save(&cfg);

    // Create tasks
    xTaskCreate(
        button_task,
        "button_task",
        2048,
        NULL,
        5,
        NULL
    );

    xTaskCreate(
        led_task,
        "led_task",
        2048,
        NULL,
        4,
        NULL
    );

    xTaskCreate(
        monitor_task,
        "monitor_task",
        4096,
        NULL,
        3,
        NULL
    );
}