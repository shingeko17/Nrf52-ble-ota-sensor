# Software Architecture Document (SAD)

## 1. System Overview

```
+--------------------------------------------------+
|                 nRF52840 DK                       |
|                                                   |
|  +---------------------------------------------+ |
|  |             Zephyr RTOS                      | |
|  |                                              | |
|  |  +-----------+  +---------+  +------------+  | |
|  |  | BLE Stack |  | Sensor  |  | MCUboot/   |  | |
|  |  | (SoftDev) |  | Driver  |  | mcumgr     |  | |
|  |  +-----------+  +---------+  +------------+  | |
|  |       |              |             |          | |
|  |  +-----------+  +---------+  +------------+  | |
|  |  | GATT Svc  |  | Temp    |  | SMP BLE    |  | |
|  |  | (custom)  |  | Reader  |  | Transport  |  | |
|  |  +-----------+  +---------+  +------------+  | |
|  |       |              |             |          | |
|  |  +------------------------------------------+| |
|  |  |            Application (main.c)           || |
|  |  | button_handler | led_status | power_mgmt  || |
|  |  +------------------------------------------+| |
|  +---------------------------------------------+ |
+--------------------------------------------------+
        |           |            |
     BLE Radio    GPIO LEDs   GPIO Buttons
```

## 2. Thread Model

| Thread | Priority | Stack | Role |
|--------|----------|-------|------|
| Main | 0 (cooperative) | 2048B | Init + temp sample loop |
| System Workqueue | -1 | 2048B | Button ISR deferred work |
| BLE RX | -8 (preemptive) | Zephyr managed | BLE stack events |
| Syswork (mcumgr) | -1 | Zephyr managed | OTA image transfer |

## 3. Data Flow

```
TEMP Peripheral → temp_sensor_read() → main loop
                                         |
                                    ble_service_notify_temp()
                                         |
                                    BLE Stack → Phone

Button GPIO IRQ → button_isr() → k_work_submit()
                                    |
                              button_work_handler()
                                    |
                              ble_service_notify_button()
                                    |
                              BLE Stack → Phone
```

## 4. Memory Map (MCUboot)

```
Flash (1MB):
0x00000000  +-------------------+
            | MCUboot (48KB)    |
0x0000C000  +-------------------+
            | Slot 0 - Active   |
            | App Image (472KB) |
0x00082000  +-------------------+
            | Slot 1 - Staging  |
            | OTA Image (472KB) |
0x000F8000  +-------------------+
            | Scratch (32KB)    |
0x00100000  +-------------------+

RAM (256KB):
0x20000000  +-------------------+
            | Zephyr Kernel     |
            | BLE Stack         |
            | App Threads       |
            | Heap (4KB)        |
0x20040000  +-------------------+
```

## 5. Boot Sequence

```
1. MCUboot starts → validates Slot 0 image signature
2. Jump to Zephyr kernel
3. Kernel init → scheduler starts
4. main() runs:
   a. led_status_init()
   b. button_handler_init()
   c. temp_sensor_init()
   d. ble_service_init() → bt_enable()
   e. power_mgmt_init()
   f. ble_service_start_adv()
   g. Loop: read temp → notify → sleep 2s
```
