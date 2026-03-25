# Product Requirements Document (PRD)

## Product: nRF52840 BLE Sensor with OTA Update

### 1. Overview
A BLE-connected sensor device on nRF52840 DK that reads onboard temperature, reports button events, indicates status via LEDs, and supports OTA firmware update.

### 2. Functional Requirements

| ID | Requirement | Priority |
|----|-------------|----------|
| FR-01 | Device shall advertise as "nRF52_OTA_Sensor" via BLE 5.0 | Must |
| FR-02 | Device shall read internal temperature every 2s | Must |
| FR-03 | Device shall notify connected central of temperature changes | Must |
| FR-04 | Device shall detect 4 button press/release events | Must |
| FR-05 | Device shall notify connected central of button events | Must |
| FR-06 | Device shall indicate state via 4 LEDs (adv/conn/OTA/error) | Must |
| FR-07 | Device shall support OTA firmware update via SMP/mcumgr | Must |
| FR-08 | Device shall use MCUboot secure bootloader with image signing | Must |
| FR-09 | Device shall enter low-power mode when idle | Should |
| FR-10 | Device shall auto-restart advertising after disconnection | Must |

### 3. Non-Functional Requirements

| ID | Requirement | Target |
|----|-------------|--------|
| NFR-01 | BLE connection latency | < 100ms |
| NFR-02 | Temperature accuracy | ± 2°C (onboard sensor limit) |
| NFR-03 | OTA update time (100KB image) | < 60s |
| NFR-04 | Idle current consumption | < 5uA (System ON sleep) |
| NFR-05 | Max concurrent connections | 1 |

### 4. Hardware
- Board: nRF52840 DK (PCA10056)
- No external sensors (uses onboard TEMP peripheral)

### 5. Success Criteria
- BLE connection + temperature notification works on nRF Connect app
- OTA update succeeds via nRF Device Manager app
- Device survives 24h continuous operation without crash
