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

#include "quantum.h"
#include "keychron_task.h"
#include "print.h"

#define POWER_ON_LED_DURATION 3000
static uint32_t power_on_indicator_timer = 0;
#ifdef RGB_MATRIX_ENABLE
static uint8_t rgb_matrix_last_mode = 0;
#endif

void keyboard_post_init_kb(void) {
    power_on_indicator_timer = timer_read32();
    gpio_write_pin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_last_mode = rgb_matrix_is_enabled() ? rgb_matrix_get_mode() : 0;
#endif
    keyboard_post_init_user();
}

bool keychron_task_kb(void) {
    if (power_on_indicator_timer) {
        if (timer_elapsed32(power_on_indicator_timer) > POWER_ON_LED_DURATION) {
            power_on_indicator_timer = 0;
            if (!host_keyboard_led_state().caps_lock) {
                gpio_write_pin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
            }
        } else {
            gpio_write_pin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
        }
    }
#ifdef RGB_MATRIX_ENABLE
    if (rgb_matrix_is_enabled()) {
        uint8_t current_mode = rgb_matrix_get_mode();
        if (current_mode != rgb_matrix_last_mode) {
            rgb_matrix_last_mode = current_mode;
            uprintf("rgb matrix mode changed: %u\n", current_mode);
        }
    }
#endif
    return true;
}

void suspend_wakeup_init_kb(void) {
    clear_keyboard();
    send_keyboard_report();
}
