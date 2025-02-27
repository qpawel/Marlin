/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

/**
 * MRR ESPA pin assignments
 * MRR ESPA is a 3D printer control board based on the ESP32 microcontroller.
 * Supports 4 stepper drivers, heated bed, single hotend.
 */

#ifndef ARDUINO_ARCH_ESP32
  "Oops! Select an ESP32 board in 'Tools > Board.'"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME "MRR ESPA"
#endif

//
// Disable I2S stepper stream
//
#ifdef I2S_STEPPER_STREAM
  #undef I2S_STEPPER_STREAM
#endif
#define I2S_WS              -1
#define I2S_BCK             -1
#define I2S_DATA            -1

//
// Limit Switches
//
#define X_MIN_PIN          34
#define Y_MIN_PIN          35
#define Z_MIN_PIN          15

//
// Steppers
//
#define X_STEP_PIN         27
#define X_DIR_PIN          26
#define X_ENABLE_PIN       25
//#define X_CS_PIN           21

#define Y_STEP_PIN         33
#define Y_DIR_PIN          32
#define Y_ENABLE_PIN       X_ENABLE_PIN
//#define Y_CS_PIN           22

#define Z_STEP_PIN         14
#define Z_DIR_PIN          12
#define Z_ENABLE_PIN       X_ENABLE_PIN
//#define Z_CS_PIN            5 // SS_PIN

#define E0_STEP_PIN        16
#define E0_DIR_PIN         17
#define E0_ENABLE_PIN      X_ENABLE_PIN
//#define E0_CS_PIN          21

//
// Temperature Sensors
//
#define TEMP_0_PIN         36   // Analog Input
#define TEMP_BED_PIN       39   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN        2
#define FAN_PIN            13
#define HEATER_BED_PIN      4

//
// MicroSD card
//
#define MOSI_PIN           23
#define MISO_PIN           19
#define SCK_PIN            18
#define SS_PIN              5
#define SDSS                5
