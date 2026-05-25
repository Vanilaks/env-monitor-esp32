#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/**
 * @brief Configuration structure
 * Holds all user-configurable parameters
 */
typedef struct {
    uint32_t report_interval_ms;  // Reporting interval in milliseconds
    float temp_high;              // High temperature threshold (Celsius)
    float temp_low;               // Low temperature threshold (Celsius)
} config_t;

/**
 * @brief Save configuration to NVS (non-volatile storage)
 * @param cfg Pointer to config struct to save
 */
void config_save(const config_t *cfg);

/**
 * @brief Load configuration from NVS
 * @param cfg Pointer to config struct to fill
 */
void config_load(config_t *cfg);

#endif // CONFIG_H