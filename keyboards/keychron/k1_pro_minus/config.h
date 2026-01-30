/* Copyright 2023 ~ 2025 @ Keychron (https://www.keychron.com)
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

// Shift register pins for matrix scanning (74HC595)
#define HC595_STCP B0
#define HC595_SHCP A1
#define HC595_DS A7

// Shift register column range (columns 1-16 are via shift register, column 0 is direct)
#define HC595_START_INDEX 1
#define HC595_END_INDEX 16

/* Indicator LEDs (accent LEDs, originally for BT/battery status) */
#define LED_BAT_LOW_PIN A4          // Red LED (active high) - originally battery low
#define LED_BAT_LOW_ON_STATE 1
// Note: BT LED on H3 is directly connected to BT module, not controllable by MCU

/*
 * Original K1 Pro hardware pins (for reference):
 *
 * LEDs:
 *   - Caps Lock LED:      A0  (on state: 1)
 *   - Bluetooth LED:      H3  (on state: 1) - used for BT connection status
 *   - Battery Low LED:    A4  (on state: 1)
 *
 * DIP Switches:
 *   - Mac/Win switch:     A8  - switches default layer (active=Mac/Layer0, inactive=Win/Layer2)
 *   - USB/BT mode switch: A10 - selects transport (0=Bluetooth, 1=USB)
 *
 * Bluetooth module (CKBT51):
 *   - Reset pin:          A9
 *   - INT input:          A5
 *   - BT INT input:       A6
 *   - USB power sense:    B1  (connected level: 0)
 */

// Increase I2C speed to 1000 KHz
#define I2C1_TIMINGR_PRESC 0U
#define I2C1_TIMINGR_SCLDEL 3U
#define I2C1_TIMINGR_SDADEL 0U
#define I2C1_TIMINGR_SCLH 15U
#define I2C1_TIMINGR_SCLL 51U

// Emulated EEPROM configuration
#define WEAR_LEVELING_LOGICAL_SIZE 2048
#define WEAR_LEVELING_BACKING_SIZE (WEAR_LEVELING_LOGICAL_SIZE * 2)
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR 2047
