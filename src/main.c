#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sensor/sensor.h"
#include "config/config.h"
#include "comms/comms.h"
#include "driver/gpio.h"

#define BUTTON_PIN GPIO_NUM_0
#define LED_PIN    GPIO_NUM_2

typedef enum {
    MODE_CONFIG,
    MODE_WORKING
} device_mode_t;

static device_mode_t current_mode = MODE_CONFIG;

static void button_task(void *arg) {
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    int last_state = 1;
    while (1) {
        int state = gpio_get_level(BUTTON_PIN);
        if (last_state == 1 && state == 0) {
            if (current_mode == MODE_CONFIG) {
                printf("Switching to WORKING MODE\n");
                current_mode = MODE_WORKING;
            } else {
                printf("Switching to CONFIG MODE\n");
                current_mode = MODE_CONFIG;
            }
            vTaskDelay(pdMS_TO_TICKS(300));
        }
        last_state = state;
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

static void led_task(void *arg) {
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    while (1) {
        if (current_mode == MODE_CONFIG) {
            gpio_set_level(LED_PIN, 1);
        } else {
            gpio_set_level(LED_PIN, !gpio_get_level(LED_PIN));
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

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
                    snprintf(msg, sizeof(msg),
                        "ALERT HIGH: %.2f C\n", temp);
                } else if (temp < cfg.temp_low) {
                    snprintf(msg, sizeof(msg),
                        "ALERT LOW: %.2f C\n", temp);
                } else {
                    snprintf(msg, sizeof(msg),
                        "TEMP: %.2f C\n", temp);
                }
                comms_send(msg);
            }
            vTaskDelay(pdMS_TO_TICKS(cfg.report_interval_ms));
        } else {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

void app_main(void) {
    printf("ENV MONITOR STARTING...\n");
    sensor_init();
    comms_init();
    config_t cfg = {
        .report_interval_ms = 5000,
        .temp_high = 35.0f,
        .temp_low = 10.0f
    };
    config_save(&cfg);
    xTaskCreate(button_task,  "button",  2048, NULL, 5, NULL);
    xTaskCreate(led_task,     "led",     2048, NULL, 4, NULL);
    xTaskCreate(monitor_task, "monitor", 4096, NULL, 3, NULL);
}