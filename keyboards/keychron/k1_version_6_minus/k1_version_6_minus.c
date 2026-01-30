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
    #include "factory_test.h"
    #include "keychron_common.h"
#endif

#include "print.h"

#define POWER_ON_LED_DURATION 3000
static uint32_t power_on_indicator_timer;

bool dip_switch_update_kb(uint8_t index, bool active) {
    if (index == 0) {
        default_layer_set(1UL << (active ? 0 : 2));
    }
    dip_switch_update_user(index, active);

    return true;
}

void keyboard_post_init_kb(void) {
    // Caps lock LED - initially OFF
    gpio_set_pin_output_push_pull(LED_CAPS_LOCK_PIN);
    gpio_write_pin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);

    // Green/Power LED (A3) - turn ON at startup (stays on permanently like original)
//    gpio_set_pin_output_push_pull(LED_POWER_PIN);
//    gpio_write_pin(LED_POWER_PIN, LED_PIN_ON_STATE);

    // Red LED (B11) - initially OFF (original only turns on with wireless enabled)
    gpio_set_pin_output_push_pull(LED_BAT_LOW_PIN);
    gpio_write_pin(LED_BAT_LOW_PIN, !LED_BAT_LOW_ON_STATE);

    // Blue LED (B15) - turn ON at startup for power-on indication
//    gpio_set_pin_output_push_pull(LED_INDICATOR_PIN);
//    gpio_write_pin(LED_INDICATOR_PIN, LED_INDICATOR_ON_STATE);

    gpio_set_pin_input_low(B2);

    power_on_indicator_timer = timer_read32();
    keyboard_post_init_user();
}

bool keychron_task_kb(void) {
    if (power_on_indicator_timer) {
        if (timer_elapsed32(power_on_indicator_timer) > POWER_ON_LED_DURATION) {
            power_on_indicator_timer = 0;

            if (!host_keyboard_led_state().caps_lock) gpio_write_pin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);

            // Turn off blue indicator LED after power-on duration
//            gpio_write_pin(LED_INDICATOR_PIN, !LED_INDICATOR_ON_STATE);
            
            // Ensure Green/Power LED (A3) is turned ON and stays ON
//            gpio_write_pin(LED_POWER_PIN, LED_PIN_ON_STATE);
            
            // Log for debugging
            uprintf("Power-on duration finished. BLE off, Green on.\n");
        } else {
            gpio_write_pin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
            
            // Log while timer is running
            static uint32_t last_log = 0;
            if (timer_elapsed32(last_log) > 1000) {
                last_log = timer_read32();
                uprintf("Power-on timer: %lu\n", timer_elapsed32(power_on_indicator_timer));
            }
        }
    }

    return true;
}
