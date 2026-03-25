# Project Summary - nRF52840 BLE OTA Sensor

## Thông tin chung
- **Repo:** https://github.com/shingeko17/Nrf52-ble-ota-sensor
- **Board:** nRF52840 DK (PCA10056) - Cortex-M4F, 64MHz, 1MB Flash, 256KB RAM
- **SDK:** nRF Connect SDK v2.5.0 (Zephyr RTOS v3.4.99)
- **Toolchain:** GCC ARM 15.2 (gnuarmemb)
- **Author:** Buzz - HCMUT

## Tính năng
| Feature | Trạng thái | Mô tả |
|---------|-----------|-------|
| BLE GATT Service | ✅ Done | Custom UUID, Temperature + Button characteristics |
| Temperature Sensor | ✅ Done | Onboard nRF52840 TEMP, đọc mỗi 2s, notify BLE |
| Button Handler | ✅ Done | 4 buttons, ISR → k_work → BLE notify |
| LED Status | ✅ Done | Advertising/Connected/OTA/Error states |
| MCUboot Bootloader | ✅ Done | Secure boot, image signing (EC-P256) |
| OTA Update | ✅ Done | SMP/mcumgr over BLE, tương thích nRF Device Manager |
| Power Management | ✅ Done | Zephyr auto idle sleep (System ON) |

## Cấu trúc project
```
nrf52k/
├── src/
│   ├── main.c              # Entry point, init + temp loop
│   ├── ble_service.c/h     # BLE GATT custom service
│   ├── temp_sensor.c/h     # Onboard temperature reader
│   ├── button_handler.c/h  # GPIO interrupt + k_work
│   ├── led_status.c/h      # LED state machine
│   └── power_mgmt.c/h      # PM init
├── include/
│   └── app_config.h        # UUIDs, intervals, macros
├── docs/
│   ├── PRD.md              # Product Requirements
│   ├── SAD.md              # Software Architecture
│   ├── SWCDD.md            # Component Design
│   └── CODING_RULES.md     # Coding Standards
├── prj.conf                # Kconfig (BLE + MCUboot + OTA)
├── app.overlay             # Device Tree overlay
├── CMakeLists.txt          # Build config
├── Makefile                # make build / make flash
└── README.md
```

## Build
```bash
# Yêu cầu
export ZEPHYR_BASE="C:/ncs/zephyr"
export GNUARMEMB_TOOLCHAIN_PATH="C:/Users/Admin/Downloads/stm32f4"
export ZEPHYR_TOOLCHAIN_VARIANT="gnuarmemb"

# Build
cd D:/personal_project/nrf52k
west build -b nrf52840dk_nrf52840 --pristine always

# Flash
west flash --softreset
```

## Kết quả build
```
App:     Flash 210KB/499KB (42%) | RAM 43KB/256KB (17%)
MCUboot: Flash 31KB/48KB (64%)  | RAM 17KB/256KB (7%)

Artifacts:
├── build/zephyr/merged.hex         # Full image (MCUboot + App)
├── build/zephyr/app_update.bin     # OTA update file
└── build/zephyr/dfu_application.zip # DFU package
```

## Setup đã hoàn thành
| Bước | Trạng thái |
|------|-----------|
| nRF Connect SDK v2.5.0 (`C:\ncs`) | ✅ |
| west + cmake + ninja (pip) | ✅ |
| GCC ARM toolchain | ✅ |
| west update (all dependencies) | ✅ |
| Python requirements | ✅ |
| Environment variables | ✅ |
| nRF Command Line Tools 10.24.2 | ✅ |
| GitHub CLI (gh) v2.67.0 | ✅ |
| Build firmware thành công | ✅ |
| Push lên GitHub | ✅ |

## Bước tiếp theo
1. Flash lên board: `make flash` (cắm nRF52840 DK qua USB)
2. Test BLE: Mở nRF Connect app trên điện thoại → Scan → Connect
3. Test OTA: Mở nRF Device Manager → Upload app_update.bin
4. Mở VS Code → cài extension nRF Connect → Open Folder → dev tiếp
