# ESP32 Environmental Monitor

## GitHub Repository
https://github.com/Vanilaks/env-monitor-esp32

## Wokwi Simulation
https://wokwi.com/projects/464986263837586433

## Build & Run
1. Open project in VS Code with PlatformIO
2. Click PlatformIO alien icon → Build
3. Firmware builds to `.pio/build/esp32dev/firmware.bin`

## Wokwi Simulation Guide
1. Open the Wokwi link above
2. Click green Play button to start
3. Click green button to toggle Config/Working mode
4. Watch serial monitor for temperature output

## Switch Communication Layer
- Current: UART (`src/comms/uart_comms.c`)
- To switch: create new file (e.g. `mqtt_comms.c`)
- Implement `comms_init()` and `comms_send()`

## Design Notes
The firmware uses a modular architecture with three layers:
- **Sensor layer** (`sensor/`): abstracts hardware reads
- **Config layer** (`config/`): persists settings via NVS flash
- **Comms layer** (`comms/`): swappable communication protocol

FreeRTOS tasks handle button, LED, and monitoring independently for robustness.