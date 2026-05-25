#ifndef SENSOR_H
#define SENSOR_H

#define SENSOR_ERROR -999.0f

/**
 * @brief Initialize the sensor
 */
void sensor_init(void);

/**
 * @brief Read temperature from sensor
 * @return Temperature in Celsius, or SENSOR_ERROR on failure
 */
float sensor_read_temperature(void);

#endif // SENSOR_H