#include "sensor.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// BMP180 I2C address
#define BMP180_ADDR 0x77

/**
 * @brief Initialize BMP180 sensor
 * In Wokwi simulation, we simulate the sensor
 */
void sensor_init(void) {
    printf("BMP180 sensor initialized\n");
}

/**
 * @brief Read temperature from BMP180
 * In Wokwi, this returns a simulated value
 * On real hardware, replace with actual I2C read
 * @return Temperature in Celsius or SENSOR_ERROR
 */
float sensor_read_temperature(void) {
    // Simulated temperature for Wokwi
    // On real hardware: read via I2C from BMP180
    static float sim_temp = 25.0f;
    sim_temp += 0.1f;
    if (sim_temp > 40.0f) sim_temp = 20.0f;
    
    if (sim_temp < -40.0f || sim_temp > 85.0f) {
        printf("ERROR: Temperature out of range\n");
        return SENSOR_ERROR;
    }
    return sim_temp;
}