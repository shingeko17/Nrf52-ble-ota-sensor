# nRF52840 BLE Sensor with OTA Update

A complete reference implementation of a BLE sensor device with MCUboot bootloader and OTA (Over-The-Air) firmware update capability.

## Features

- 📡 **BLE Communication**: Custom GATT service for sensor data
- 🌡️ **Temperature Sensor**: Onboard nRF52840 temperature reading
- 🔘 **Button Events**: Press detection with BLE notification
- 💡 **LED Status**: Visual feedback for device state
- 🔄 **OTA Update**: Wireless firmware update via SMP/mcumgr
- 🔐 **Secure Boot**: MCUboot with image signing
- 🔋 **Low Power**: Zephyr power management

## Hardware

- **Board**: nRF52840 DK (PCA10056)
- **MCU**: nRF52840 (Cortex-M4F, 64MHz, 1MB Flash, 256KB RAM)

## Quick Start

### Prerequisites

1. Install [nRF Connect SDK](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/installation.html) (v2.5.x or later)
2. Install VS Code with nRF Connect Extension

### Build & Flash

```bash
# Clone repository
git clone https://github.com/your-username/nrf52-ble-ota-sensor.git
cd nrf52-ble-ota-sensor

# Build with west
west build -b nrf52840dk_nrf52840

# Flash to board
west flash
```

Or use VS Code:
1. Open folder in VS Code
2. nRF Connect → Add Build Configuration → nrf52840dk_nrf52840
3. Click Build → Flash

### Test

1. Download **nRF Connect** app on your phone
2. Scan for "nRF52_OTA_Sensor"
3. Connect and explore services
4. Enable notifications on Temperature characteristic
5. Press buttons to see events

### OTA Update

1. Build new firmware version (change `APP_VERSION` in `app_config.h`)
2. Download **nRF Device Manager** app
3. Connect to device
4. Upload `build/zephyr/app_update.bin`
5. Device will reboot with new firmware

## Project Structure

```
nrf52-ble-ota-sensor/
├── src/
│   ├── main.c              # Application entry point
│   ├── ble_service.c/h     # BLE GATT service
│   ├── temp_sensor.c/h     # Temperature reading
│   ├── button_handler.c/h  # Button events
│   ├── led_status.c/h      # LED control
│   └── power_mgmt.c/h      # Power management
├── include/
│   └── app_config.h        # Configuration defines
├── docs/
│   ├── PRD.md              # Product Requirements
│   ├── SAD.md              # Software Architecture
│   ├── SWCDD.md            # Component Design
│   └── CODING_RULES.md     # Coding Standards
├── prj.conf                # Kconfig configuration
├── app.overlay             # Device tree overlay
└── README.md               # This file
```

## BLE Services

### Custom Sensor Service
- **UUID**: `12345678-1234-5678-1234-56789abcdef0`

| Characteristic | UUID | Properties | Description |
|----------------|------|------------|-------------|
| Temperature | `...def1` | Read, Notify | int16 (°C × 100) |
| Button Event | `...def2` | Notify | uint8[2] (id, state) |

### SMP Service (OTA)
- Provided by mcumgr subsystem
- Compatible with nRF Device Manager app

## LED Indicators

| LED | State | Meaning |
|-----|-------|---------|
| LED1 | Blinking slow | Advertising |
| LED2 | Solid | Connected |
| LED3 | Blinking fast | OTA in progress |
| LED4 | Solid | Error |

## Documentation

- [PRD.md](docs/PRD.md) - Product Requirements Document
- [SAD.md](docs/SAD.md) - Software Architecture Document
- [SWCDD.md](docs/SWCDD.md) - Software Component Detailed Design
- [CODING_RULES.md](docs/CODING_RULES.md) - Coding Standards

## Development Workflow

This project follows an AI-first development workflow:

1. ✅ Write PRD (Product Requirements)
2. ✅ Write SAD (Software Architecture)
3. ✅ Write SWCDD (Component Design)
4. ✅ Define Coding Rules
5. ⬜ AI Generate Code
6. ⬜ AI Review Code
7. ⬜ Human Validate
8. ⬜ Build & Flash
9. ⬜ Hardware Test
10. ⬜ Iterate

## License

MIT License - See [LICENSE](LICENSE) for details.

## Author

Buzz - Embedded Systems Student @ HCMUT

## References

- [nRF Connect SDK Documentation](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/)
- [MCUboot Documentation](https://docs.mcuboot.com/)
- [Zephyr BLE Documentation](https://docs.zephyrproject.org/latest/connectivity/bluetooth/)
