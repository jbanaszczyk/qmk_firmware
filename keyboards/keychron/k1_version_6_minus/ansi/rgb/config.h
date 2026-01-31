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

#ifdef RGB_MATRIX_ENABLE
/* RGB Matrix driver configuration */
#    define RGB_MATRIX_LED_COUNT 87

#    define DRIVER_CS_PINS {A8, C9}

/* Scan phase of led driver */
#    define SNLED27351_PHASE_CHANNEL MSKPHASE_9CHANNEL
/* Set LED driver current */
#    define SNLED27351_CURRENT_TUNE {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30}

/* Allow shutdown of led driver to save power */
#    define RGB_MATRIX_DRIVER_SHUTDOWN_ENABLE
#    define RGB_MATRIX_DRIVER_LOAD_ENABLE
#    define RGB_MATRIX_KEYPRESSES
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#endif
