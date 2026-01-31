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

#include "quantum.h"
#include "keychron_task.h"
#ifdef FACTORY_TEST_ENABLE
#    include "factory_test.h"
#    include "keychron_common.h"
#endif

#include "print.h"

#define POWER_ON_LED_DURATION 3000
static uint32_t power_on_indicator_timer;

static uint8_t rgb_matrix_last_mode = 0;

/*
bool dip_switch_update_kb(uint8_t index, bool active) {
    if (index == 0) {
        default_layer_set(1UL << (active ? 0 : 2));
    }
    dip_switch_update_user(index, active);

    return true;
}
*/

void initialize_led_indicators(void) {
    // Caps lock LED - turn ON at startup for indication
    gpio_set_pin_output_push_pull(LED_CAPS_LOCK_PIN);
    gpio_write_pin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);

    // Green/Power LED (A3) - initially OFF
    gpio_set_pin_output_push_pull(LED_POWER_PIN);
    gpio_write_pin(LED_POWER_PIN, !LED_POWER_ON_STATE);

    // Blue LED (B15) - initially OFF
    gpio_set_pin_output_push_pull(LED_INDICATOR_PIN);
    gpio_write_pin(LED_INDICATOR_PIN, !LED_INDICATOR_ON_STATE);
}
void keyboard_post_init_kb(void) {
    initialize_led_indicators();
    gpio_write_pin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);

    gpio_set_pin_input_low(B2);

#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_last_mode = rgb_matrix_is_enabled() ? rgb_matrix_get_mode() : 0;
#endif

    power_on_indicator_timer = timer_read32();
    keyboard_post_init_user();
}

bool keychron_task_kb(void) {
    if (power_on_indicator_timer) {
        if (timer_elapsed32(power_on_indicator_timer) <= POWER_ON_LED_DURATION) {
            gpio_write_pin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);

            static uint32_t last_log = 0;
            if (timer_elapsed32(last_log) > 1000) {
                last_log = timer_read32();
                uprintf("Power-on timer: %lu\n", timer_elapsed32(power_on_indicator_timer));
            }
        } else {
            power_on_indicator_timer = 0;

            if (!host_keyboard_led_state().caps_lock) {
                gpio_write_pin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
            }

            uprintf("Power-on duration finished.\n");
        }
    }

    if (rgb_matrix_is_enabled()) {
        uint8_t current_mode = rgb_matrix_get_mode();
        if (current_mode != rgb_matrix_last_mode) {
            rgb_matrix_last_mode = current_mode;
            dprintf("rgb matrix mode changed: %s\n", rgb_matrix_get_mode_name(current_mode));
        }
    }

    return true;
}

bool process_record_keychron_kb(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (keycode == KC_F12) {
        }
    }
    return true;
}
