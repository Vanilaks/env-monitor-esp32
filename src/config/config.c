#include "config.h"
#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"

#define NVS_NAMESPACE "env_cfg"

/**
 * @brief Save configuration to NVS flash
 * This ensures settings survive power cycles
 */
void config_save(const config_t *cfg) {
    nvs_flash_init();
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("ERROR: Cannot open NVS\n");
        return;
    }
    nvs_set_u32(handle, "interval", cfg->report_interval_ms);
    // Store floats as uint32 by copying bytes
    uint32_t high, low;
    memcpy(&high, &cfg->temp_high, sizeof(float));
    memcpy(&low,  &cfg->temp_low,  sizeof(float));
    nvs_set_u32(handle, "temp_high", high);
    nvs_set_u32(handle, "temp_low",  low);
    nvs_commit(handle);
    nvs_close(handle);
    printf("Config saved: interval=%lums high=%.1f low=%.1f\n",
        (unsigned long)cfg->report_interval_ms,
        cfg->temp_high, cfg->temp_low);
}

/**
 * @brief Load configuration from NVS flash
 * Falls back to defaults if not found
 */
void config_load(config_t *cfg) {
    nvs_flash_init();
    nvs_handle_t handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        printf("No saved config, using defaults\n");
        cfg->report_interval_ms = 5000;
        cfg->temp_high = 35.0f;
        cfg->temp_low  = 10.0f;
        return;
    }
    uint32_t high, low;
    nvs_get_u32(handle, "interval",  &cfg->report_interval_ms);
    nvs_get_u32(handle, "temp_high", &high);
    nvs_get_u32(handle, "temp_low",  &low);
    memcpy(&cfg->temp_high, &high, sizeof(float));
    memcpy(&cfg->temp_low,  &low,  sizeof(float));
    nvs_close(handle);
    printf("Config loaded: interval=%lums high=%.1f low=%.1f\n",
        (unsigned long)cfg->report_interval_ms,
        cfg->temp_high, cfg->temp_low);
}