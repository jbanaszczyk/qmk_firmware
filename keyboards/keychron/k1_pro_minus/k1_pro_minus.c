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

    // Initialize indicator LED (accent LED)
    gpio_set_pin_output_push_pull(LED_BAT_LOW_PIN);
    gpio_write_pin(LED_BAT_LOW_PIN, LED_BAT_LOW_ON_STATE);  // Turn on red LED at startup

#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_last_mode = rgb_matrix_is_enabled() ? rgb_matrix_get_mode() : 0;
#endif
    keyboard_post_init_user();
}

#ifdef RGB_MATRIX_ENABLE
static void print_rgb_matrix_mode_details(uint8_t mode) {
    switch (mode) {
        case RGB_MATRIX_NONE:
            uprintf("  RGB_MATRIX_NONE                      No effect\n");
            break;
        case RGB_MATRIX_SOLID_COLOR:
            uprintf("  RGB_MATRIX_SOLID_COLOR               Static single hue, no speed support\n");
            break;
        case RGB_MATRIX_ALPHAS_MODS:
            uprintf("  RGB_MATRIX_ALPHAS_MODS               Static dual hue, speed is hue for secondary hue\n");
            break;
        case RGB_MATRIX_GRADIENT_UP_DOWN:
            uprintf("  RGB_MATRIX_GRADIENT_UP_DOWN          Static gradient top to bottom, speed controls how much gradient changes\n");
            break;
        case RGB_MATRIX_GRADIENT_LEFT_RIGHT:
            uprintf("  RGB_MATRIX_GRADIENT_LEFT_RIGHT       Static gradient left to right, speed controls how much gradient changes\n");
            break;
        case RGB_MATRIX_BREATHING:
            uprintf("  RGB_MATRIX_BREATHING                 Single hue brightness cycling animation\n");
            break;
        case RGB_MATRIX_BAND_SAT:
            uprintf("  RGB_MATRIX_BAND_SAT                  Single hue band fading saturation scrolling left to right\n");
            break;
        case RGB_MATRIX_BAND_VAL:
            uprintf("  RGB_MATRIX_BAND_VAL                  Single hue band fading brightness scrolling left to right\n");
            break;
        case RGB_MATRIX_BAND_PINWHEEL_SAT:
            uprintf("  RGB_MATRIX_BAND_PINWHEEL_SAT         Single hue 3 blade spinning pinwheel fades saturation\n");
            break;
        case RGB_MATRIX_BAND_PINWHEEL_VAL:
            uprintf("  RGB_MATRIX_BAND_PINWHEEL_VAL         Single hue 3 blade spinning pinwheel fades brightness\n");
            break;
        case RGB_MATRIX_BAND_SPIRAL_SAT:
            uprintf("  RGB_MATRIX_BAND_SPIRAL_SAT           Single hue spinning spiral fades saturation\n");
            break;
        case RGB_MATRIX_BAND_SPIRAL_VAL:
            uprintf("  RGB_MATRIX_BAND_SPIRAL_VAL           Single hue spinning spiral fades brightness\n");
            break;
        case RGB_MATRIX_CYCLE_ALL:
            uprintf("  RGB_MATRIX_CYCLE_ALL                 Full keyboard solid hue cycling through full gradient\n");
            break;
        case RGB_MATRIX_CYCLE_LEFT_RIGHT:
            uprintf("  RGB_MATRIX_CYCLE_LEFT_RIGHT          Full gradient scrolling left to right\n");
            break;
        case RGB_MATRIX_CYCLE_UP_DOWN:
            uprintf("  RGB_MATRIX_CYCLE_UP_DOWN             Full gradient scrolling top to bottom\n");
            break;
        case RGB_MATRIX_CYCLE_OUT_IN:
            uprintf("  RGB_MATRIX_CYCLE_OUT_IN              Full gradient scrolling out to in\n");
            break;
        case RGB_MATRIX_CYCLE_OUT_IN_DUAL:
            uprintf("  RGB_MATRIX_CYCLE_OUT_IN_DUAL         Full dual gradients scrolling out to in\n");
            break;
        case RGB_MATRIX_RAINBOW_MOVING_CHEVRON:
            uprintf("  RGB_MATRIX_RAINBOW_MOVING_CHEVRON    Full gradient Chevron shapped scrolling left to right\n");
            break;
        case RGB_MATRIX_CYCLE_PINWHEEL:
            uprintf("  RGB_MATRIX_CYCLE_PINWHEEL            Full gradient spinning pinwheel around center of keyboard\n");
            break;
        case RGB_MATRIX_CYCLE_SPIRAL:
            uprintf("  RGB_MATRIX_CYCLE_SPIRAL              Full gradient spinning spiral around center of keyboard\n");
            break;
        case RGB_MATRIX_DUAL_BEACON:
            uprintf("  RGB_MATRIX_DUAL_BEACON               Full gradient spinning around center of keyboard\n");
            break;
        case RGB_MATRIX_RAINBOW_BEACON:
            uprintf("  RGB_MATRIX_RAINBOW_BEACON            Full tighter gradient spinning around center of keyboard\n");
            break;
        case RGB_MATRIX_RAINBOW_PINWHEELS:
            uprintf("  RGB_MATRIX_RAINBOW_PINWHEELS         Full dual gradients spinning two halves of keyboard\n");
            break;
        case RGB_MATRIX_FLOWER_BLOOMING:
            uprintf("  RGB_MATRIX_FLOWER_BLOOMING           Full tighter gradient of first half scrolling left to right and second half scrolling right to left\n");
            break;
        case RGB_MATRIX_RAINDROPS:
            uprintf("  RGB_MATRIX_RAINDROPS                 Randomly changes a single key's hue\n");
            break;
        case RGB_MATRIX_JELLYBEAN_RAINDROPS:
            uprintf("  RGB_MATRIX_JELLYBEAN_RAINDROPS       Randomly changes a single key's hue and saturation\n");
            break;
        case RGB_MATRIX_HUE_BREATHING:
            uprintf("  RGB_MATRIX_HUE_BREATHING             Hue shifts up a slight amount at the same time, then shifts back\n");
            break;
        case RGB_MATRIX_HUE_PENDULUM:
            uprintf("  RGB_MATRIX_HUE_PENDULUM              Hue shifts up a slight amount in a wave to the right, then back to the left\n");
            break;
        case RGB_MATRIX_HUE_WAVE:
            uprintf("  RGB_MATRIX_HUE_WAVE                  Hue shifts up a slight amount and then back down in a wave to the right\n");
            break;
        case RGB_MATRIX_PIXEL_FRACTAL:
            uprintf("  RGB_MATRIX_PIXEL_FRACTAL             Single hue fractal filled keys pulsing horizontally out to edges\n");
            break;
        case RGB_MATRIX_PIXEL_FLOW:
            uprintf("  RGB_MATRIX_PIXEL_FLOW                Pulsing RGB flow along LED wiring with random hues\n");
            break;
        case RGB_MATRIX_PIXEL_RAIN:
            uprintf("  RGB_MATRIX_PIXEL_RAIN                Randomly light keys with random hues\n");
            break;
        case RGB_MATRIX_TYPING_HEATMAP:
            uprintf("  RGB_MATRIX_TYPING_HEATMAP            How hot is your WPM!\n");
            break;
        case RGB_MATRIX_DIGITAL_RAIN:
            uprintf("  RGB_MATRIX_DIGITAL_RAIN              That famous computer simulation\n");
            break;
        case RGB_MATRIX_SOLID_REACTIVE_SIMPLE:
            uprintf("  RGB_MATRIX_SOLID_REACTIVE_SIMPLE     Pulses keys hit to hue & value then fades value out\n");
            break;
        case RGB_MATRIX_SOLID_REACTIVE:
            uprintf("  RGB_MATRIX_SOLID_REACTIVE            Static single hue, pulses keys hit to shifted hue then fades to current hue\n");
            break;
        case RGB_MATRIX_SOLID_REACTIVE_WIDE:
            uprintf("  RGB_MATRIX_SOLID_REACTIVE_WIDE       Hue & value pulse near a single key hit then fades value out\n");
            break;
        case RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE:
            uprintf("  RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE  Hue & value pulse near multiple key hits then fades value out\n");
            break;
        case RGB_MATRIX_SOLID_REACTIVE_CROSS:
            uprintf("  RGB_MATRIX_SOLID_REACTIVE_CROSS      Hue & value pulse the same column and row of a single key hit then fades value out\n");
            break;
        case RGB_MATRIX_SOLID_REACTIVE_MULTICROSS:
            uprintf("  RGB_MATRIX_SOLID_REACTIVE_MULTICROSS Hue & value pulse the same column and row of multiple key hits then fades value out\n");
            break;
        case RGB_MATRIX_SOLID_REACTIVE_NEXUS:
            uprintf("  RGB_MATRIX_SOLID_REACTIVE_NEXUS      Hue & value pulse away on the same column and row of a single key hit then fades value out\n");
            break;
        case RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS:
            uprintf("  RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS Hue & value pulse away on the same column and row of multiple key hits then fades value out\n");
            break;
        case RGB_MATRIX_SPLASH:
            uprintf("  RGB_MATRIX_SPLASH                    Full gradient & value pulse away from a single key hit then fades value out\n");
            break;
        case RGB_MATRIX_MULTISPLASH:
            uprintf("  RGB_MATRIX_MULTISPLASH               Full gradient & value pulse away from multiple key hits then fades value out\n");
            break;
        case RGB_MATRIX_SOLID_SPLASH:
            uprintf("  RGB_MATRIX_SOLID_SPLASH              Hue & value pulse away from a single key hit then fades value out\n");
            break;
        case RGB_MATRIX_SOLID_MULTISPLASH:
            uprintf("  RGB_MATRIX_SOLID_MULTISPLASH         Hue & value pulse away from multiple key hits then fades value out\n");
            break;
        case RGB_MATRIX_STARLIGHT:
            uprintf("  RGB_MATRIX_STARLIGHT                 LEDs turn on and off at random at varying brightness, maintaining user set color\n");
            break;
//        case RGB_MATRIX_STARLIGHT_SMOOTH:
//            uprintf("  RGB_MATRIX_STARLIGHT_SMOOTH          LEDs slowly increase and decrease in brightness randomly\n");
//            break;
        case RGB_MATRIX_STARLIGHT_DUAL_HUE:
            uprintf("  RGB_MATRIX_STARLIGHT_DUAL_HUE        LEDs turn on and off at random at varying brightness, modifies user set hue by +- 30\n");
            break;
        case RGB_MATRIX_STARLIGHT_DUAL_SAT:
            uprintf("  RGB_MATRIX_STARLIGHT_DUAL_SAT        LEDs turn on and off at random at varying brightness, modifies user set saturation by +- 30\n");
            break;
        case RGB_MATRIX_RIVERFLOW:
            uprintf("  RGB_MATRIX_RIVERFLOW                 Modification to breathing animation, offset's animation depending on key location to simulate a river flowing\n");
            break;
        default:
            uprintf("  Unknown mode\n");
            break;
    }
}
#endif

bool keychron_task_kb(void) {
    if (power_on_indicator_timer) {
        if (timer_elapsed32(power_on_indicator_timer) > POWER_ON_LED_DURATION) {
            power_on_indicator_timer = 0;
            if (!host_keyboard_led_state().caps_lock) {
                gpio_write_pin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
            }
            // Turn off indicator LED after power-on duration
            gpio_write_pin(LED_BAT_LOW_PIN, !LED_BAT_LOW_ON_STATE);
        } else {
            gpio_write_pin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
        }
    }
#ifdef RGB_MATRIX_ENABLE
    if (rgb_matrix_is_enabled()) {
        uint8_t current_mode = rgb_matrix_get_mode();
        if (current_mode != rgb_matrix_last_mode) {
            rgb_matrix_last_mode = current_mode;
            uprintf("rgb matrix mode changed: %u   ", current_mode);
            print_rgb_matrix_mode_details(current_mode);
        }
    }
#endif
    return true;
}

void suspend_wakeup_init_kb(void) {
    clear_keyboard();
    send_keyboard_report();
}
