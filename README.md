# ESP32 Environmental Monitor

## GitHub Repository
https://github.com/Vanilaks/env-monitor-esp32

## Wokwi Simulation
https://wokwi.com/projects/464993114184855553

## Build and Run
1. Install VS Code with PlatformIO extension
2. Clone the repository
3. Open project in VS Code
4. Click PlatformIO alien icon → Build
5. Firmware builds to `.pio/build/esp32dev/firmware.bin`

## Simulate Button Presses and Sensor Data in Wokwi
1. Open https://wokwi.com/projects/464986263837586433
2. Click green Play button to start simulation
3. Device starts in CONFIG MODE (LED solid ON)
4. Click the green pushbutton to switch to WORKING MODE
5. LED starts blinking in WORKING MODE
6. Temperature readings appear in Serial Monitor every 5 seconds
7. If temp > 35°C → ALERT HIGH message
8. If temp < 10°C → ALERT LOW message

## Switch the Communication Layer
The communication layer is fully abstracted in `src/comms/`:
- Current implementation: `src/comms/uart_comms.c`
- To switch to a different protocol (e.g. MQTT, BLE):
  1. Create a new file e.g. `src/comms/mqtt_comms.c`
  2. Implement the same two functions:
     - `void comms_init(void)`
     - `void comms_send(const char *message)`
  3. Replace `uart_comms.c` in the build
  4. No changes needed to `main.c` or any other file

## Test Evidence
- Build SUCCESS screenshot: `test_evidence.png`
- All unit tests pass (see `src/tests/test_logic.c`):
  - `test_default_config` — validates default config values
  - `test_threshold_logic` — validates high/low threshold logic
  - `test_sensor_error_value` — validates SENSOR_ERROR constant

## Design Notes (Architecture)

The firmware is built around three independent layers:

**1. Sensor Layer (`src/sensor/`)**
The `sensor.h` header defines a clean interface (`sensor_init()`, `sensor_read_temperature()`). The implementation `bmp180.c` handles the BMP180 hardware. To swap sensors, only this file needs replacing — core logic is untouched.

**2. Configuration Layer (`src/config/`)**
Uses ESP-IDF NVS (Non-Volatile Storage) flash to persist settings across power cycles. Stores report interval and temperature thresholds. Falls back to safe defaults if no saved config exists.

**3. Communication Layer (`src/comms/`)**
Abstracts all output behind `comms_init()` and `comms_send()`. Currently implemented with UART. Swapping to MQTT or BLE requires only a new implementation file — zero changes to business logic.

**FreeRTOS Tasks**
Three independent tasks run concurrently:
- `button_task` — debounced button input, toggles modes
- `led_task` — solid in CONFIG, blinking in WORKING mode
- `monitor_task` — reads sensor and sends alerts on interval

This design ensures robustness: a sensor failure only affects `monitor_task` and won't crash the entire system.