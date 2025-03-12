# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

# Add path to the extra module bme68x_iaq
list(APPEND ZEPHYR_EXTRA_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}/drivers/sensor
)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(EnvironmentalSensor)

target_sources(app PRIVATE src/main.cxx src/ble.c src/led.c src/sensor.cxx src/battery.c)
