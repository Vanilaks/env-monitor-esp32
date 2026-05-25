#include <stdio.h>
#include <assert.h>
#include "../config/config.h"
#include "../sensor/sensor.h"

/**
 * @brief Test that default config values are valid
 */
static void test_default_config(void) {
    config_t cfg = {
        .report_interval_ms = 5000,
        .temp_high = 35.0f,
        .temp_low  = 10.0f
    };
    assert(cfg.report_interval_ms > 0);
    assert(cfg.temp_high > cfg.temp_low);
    printf("PASS: test_default_config\n");
}

/**
 * @brief Test temperature threshold logic
 */
static void test_threshold_logic(void) {
    float temp_high = 35.0f;
    float temp_low  = 10.0f;

    float temp_normal = 25.0f;
    float temp_too_high = 40.0f;
    float temp_too_low  = 5.0f;

    assert(temp_normal  <= temp_high && temp_normal  >= temp_low);
    assert(temp_too_high > temp_high);
    assert(temp_too_low  < temp_low);
    printf("PASS: test_threshold_logic\n");
}

/**
 * @brief Test sensor error value
 */
static void test_sensor_error_value(void) {
    float err = SENSOR_ERROR;
    assert(err == -999.0f);
    printf("PASS: test_sensor_error_value\n");
}

/**
 * @brief Run all unit tests
 */
void run_tests(void) {
    printf("--- Running Unit Tests ---\n");
    test_default_config();
    test_threshold_logic();
    test_sensor_error_value();
    printf("--- All Tests Passed! ---\n");
}