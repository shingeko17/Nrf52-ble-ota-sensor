# Software Component Detailed Design (SWCDD)

## 1. Module: ble_service

**Files:** `src/ble_service.c`, `src/ble_service.h`

**Responsibility:** BLE GATT server with custom sensor service

**GATT Structure:**
```
Custom Sensor Service (UUID: 12345678-1234-5678-1234-56789abcdef0)
├── Temperature Characteristic (UUID: ...def1)
│   ├── Properties: Read, Notify
│   ├── Type: int16_t (°C × 100)
│   └── CCC Descriptor
└── Button Event Characteristic (UUID: ...def2)
    ├── Properties: Notify
    ├── Type: uint8_t[2] (button_id, pressed)
    └── CCC Descriptor
```

**State Management:**
- `current_conn`: Active BLE connection reference
- `temp_notify_enabled`: CCC subscription flag
- `button_notify_enabled`: CCC subscription flag

**API:**
| Function | Description |
|----------|-------------|
| `ble_service_init()` | Call `bt_enable()`, register GATT |
| `ble_service_start_adv()` | Start BLE advertising |
| `ble_service_is_connected()` | Check connection state |
| `ble_service_notify_temp(int16_t)` | Send temp notification |
| `ble_service_notify_button(uint8_t, bool)` | Send button notification |

---

## 2. Module: temp_sensor

**Files:** `src/temp_sensor.c`, `src/temp_sensor.h`

**Responsibility:** Read nRF52840 onboard TEMP peripheral

**Implementation:**
- Uses Zephyr Sensor API (`sensor_sample_fetch` / `sensor_channel_get`)
- Device: `DT_NODELABEL(temp)` (nRF internal temperature)
- Output: int16_t in °C × 100 (e.g., 2350 = 23.50°C)

---

## 3. Module: button_handler

**Files:** `src/button_handler.c`, `src/button_handler.h`

**Responsibility:** Detect 4 button press/release, notify BLE

**Design Pattern:** ISR → k_work → BLE notify
- GPIO interrupt (ISR context) captures edge
- `k_work_submit()` defers processing to system workqueue
- Work handler calls `ble_service_notify_button()`

**Why deferred?** BLE GATT notify cannot be called from ISR context.

---

## 4. Module: led_status

**Files:** `src/led_status.c`, `src/led_status.h`

**Responsibility:** Visual state indication via 4 LEDs

**State Machine:**
```
         ┌─── IDLE (all off)
         |
    bt_enable()
         |
         v
    ADVERTISING ←── disconnected()
    (LED1 blink)        |
         |              |
    connected()         |
         |              |
         v              |
    CONNECTED ──────────┘
    (LED2 solid)
         |
    OTA start
         |
         v
       OTA
    (LED3 fast blink)
         |
    OTA done → CONNECTED

    Any error → ERROR (LED4 solid)
```

---

## 5. Module: power_mgmt

**Files:** `src/power_mgmt.c`, `src/power_mgmt.h`

**Responsibility:** Low-power idle management

**Implementation:** Zephyr automatic idle sleep (`CONFIG_PM=y`).
When all threads are sleeping (`k_msleep`), kernel enters System ON sleep (WFI).
Wake on: BLE event, GPIO interrupt, kernel timer.

---

## 6. Module: main

**File:** `src/main.c`

**Responsibility:** Init all modules, run temperature sample loop

**Sequence:**
1. Init LED → Button → Temp → BLE → Power
2. Start advertising
3. Loop forever: read temp → notify if connected → sleep 2s
