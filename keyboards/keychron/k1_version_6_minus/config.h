/* Copyright 2024 ~ 2025 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

/* Indicator LEDs (Source of Truth) */
#define LED_INDICATOR_PIN B15       // Blue LED
#define LED_INDICATOR_ON_STATE 0    // Active Low
#define LED_POWER_PIN A3            // Green/Power LED
#define LED_POWER_ON_STATE 0        // Active Low
// Caps Lock is handled via info.json: A13, active high (1), generates LED_CAPS_LOCK_PIN and LED_PIN_ON_STATE

/* I2C Driver Configuration */
#define I2C1_SCL_PIN B8
#define I2C1_SDA_PIN B9
#define I2C1_CLOCK_SPEED 400000
#define I2C1_DUTY_CYCLE FAST_DUTY_CYCLE_2

#define I2C1_OPMODE OPMODE_I2C
#define DYNAMIC_KEYMAP_BUFFER_ENABLE

#if defined(RGB_MATRIX_ENABLE) || defined(LED_MATRIX_ENABLE)
/* SPI configuration */
#    define SPI_DRIVER SPIDQ
#    define SPI_SCK_PIN A5
#    define SPI_MISO_PIN A6
#    define SPI_MOSI_PIN A7

#    define SNLED27351_SDB_PIN B7
#    define SNLED27351_SPI_DIVISOR 16
#    define SNLED27351_SPI_MODE 0
#endif

/* Factory test keys */
#define FN_KEY_1 MO(1)
#define FN_KEY_2 MO(3)

#define RGB_MATRIX_MODE_NAME_ENABLE
