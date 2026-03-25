# SPDX-License-Identifier: MIT
# nRF52840 BLE OTA Sensor - CMake build configuration

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(nrf52-ble-ota-sensor
    VERSION 1.0.0
    LANGUAGES C
)

# Include paths
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Application sources
target_sources(app PRIVATE
    src/main.c
    src/ble_service.c
    src/temp_sensor.c
    src/button_handler.c
    src/led_status.c
    src/power_mgmt.c
)
