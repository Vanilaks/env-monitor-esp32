# ESP32 Environmental Monitor

## Build & Run
1. Open project in VS Code with PlatformIO
2. Press `Ctrl+Alt+B` to build
3. Upload to device or open Wokwi simulator

## Wokwi Simulation
1. Build project first (`Ctrl+Alt+B`)
2. Open `wokwi/diagram.json` in Wokwi
3. Press the green button to toggle Config/Working mode
4. Watch serial monitor for temperature output

## Switch Communication Layer
- Current: UART (`src/comms/uart_comms.c`)
- To switch: create new file (e.g. `mqtt_comms.c`)
- Implement `comms_init()` and `comms_send()`
- Replace in build

## Design Notes
The firmware uses a modular architecture with three layers:
- **Sensor layer** (`sensor/`): abstracts hardware reads
- **Config layer** (`config/`): persists settings via NVS flash
- **Comms layer** (`comms/`): swappable communication protocol

FreeRTOS tasks handle button, LED, and monitoring independently for robustness.