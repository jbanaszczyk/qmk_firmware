/* Copyright 2021 @ Keychron (https://www.keychron.com)
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

#include "snled27351_spi.h"
#include "spi_master.h"
#include "gpio.h"
#include "wait.h"
#include <string.h>

#define SNLED27351_PWM_REGISTER_COUNT 192
#define SNLED27351_LED_CONTROL_REGISTER_COUNT 24

#ifndef SNLED27351_PHASE_CHANNEL
#    define SNLED27351_PHASE_CHANNEL MSKPHASE_12CHANNEL
#endif

#ifndef SNLED27351_CURRENT_TUNE
#    define SNLED27351_CURRENT_TUNE \
        { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
#endif

#define SNLED27351_WRITE (0 << 7)
#define SNLED27351_READ (1 << 7)
#define SNLED27351_PATTERN (2 << 4)

#ifndef SNLED27351_SPI_DIVISOR
#    define SNLED27351_SPI_DIVISOR 16
#endif

#ifndef SNLED27351_SPI_MODE
#    define SNLED27351_SPI_MODE 0
#endif

pin_t cs_pins[] = DRIVER_CS_PINS;

// These buffers match the snled27351 PWM registers.
// The control buffers match the PG0 LED On/Off registers.
uint8_t g_pwm_buffer[SNLED27351_DRIVER_COUNT][SNLED27351_PWM_REGISTER_COUNT];
bool    g_pwm_buffer_update_required[SNLED27351_DRIVER_COUNT] = {false};

uint8_t g_led_control_registers[SNLED27351_DRIVER_COUNT][SNLED27351_LED_CONTROL_REGISTER_COUNT] = {0};
bool    g_led_control_registers_update_required[SNLED27351_DRIVER_COUNT] = {false};

bool snled27351_write(uint8_t index, uint8_t page, uint8_t reg, uint8_t *data, uint8_t len) {
    static uint8_t spi_transfer_buffer[2] = {0};

    if (index > SNLED27351_DRIVER_COUNT - 1) return false;

    if (!spi_start(cs_pins[index], false, SNLED27351_SPI_MODE, SNLED27351_SPI_DIVISOR)) {
        spi_stop();
        return false;
    }

    spi_transfer_buffer[0] = SNLED27351_WRITE | SNLED27351_PATTERN | (page & 0x0F);
    spi_transfer_buffer[1] = reg;

    if (spi_transmit(spi_transfer_buffer, 2) != SPI_STATUS_SUCCESS) {
        spi_stop();
        return false;
    }

    if (spi_transmit(data, len) != SPI_STATUS_SUCCESS) {
        spi_stop();
        return false;
    }

    spi_stop();
    return true;
}

bool snled27351_write_register(uint8_t index, uint8_t page, uint8_t reg, uint8_t data) {
    return snled27351_write(index, page, reg, &data, 1);
}

bool snled27351_write_pwm_buffer(uint8_t index) {
    if (g_pwm_buffer_update_required[index]) {
        snled27351_write(index, LED_PWM_PAGE, 0, g_pwm_buffer[index], SNLED27351_PWM_REGISTER_COUNT);
    }
    g_pwm_buffer_update_required[index] = false;
    return true;
}

void snled27351_init_drivers(void) {
#if defined(SNLED27351_SDB_PIN)
    gpio_set_pin_output(SNLED27351_SDB_PIN);
    gpio_write_pin_high(SNLED27351_SDB_PIN);
    wait_ms(10);
#endif

    spi_init();

    for (uint8_t i = 0; i < SNLED27351_DRIVER_COUNT; i++)
        snled27351_init(i);

    for (int index = 0; index < SNLED27351_LED_COUNT; index++) {
        snled27351_set_led_control_register(index, true, true, true);
    }

    for (uint8_t i = 0; i < SNLED27351_DRIVER_COUNT; i++)
        snled27351_update_led_control_registers(i);
}

void snled27351_init(uint8_t index) {
    gpio_set_pin_output(cs_pins[index]);
    gpio_write_pin_high(cs_pins[index]);
    
    wait_ms(1);
    
    // Setting LED driver to shutdown mode
    snled27351_write_register(index, FUNCTION_PAGE, CONFIGURATION_REG, MSKSW_SHUT_DOWN_MODE);
    // Setting internal channel pulldown/pullup
    snled27351_write_register(index, FUNCTION_PAGE, PDU_REG, MSKSET_CA_CB_CHANNEL);
    // Select number of scan phase
    snled27351_write_register(index, FUNCTION_PAGE, SCAN_PHASE_REG, SNLED27351_PHASE_CHANNEL);
    // Setting PWM Delay Phase
    snled27351_write_register(index, FUNCTION_PAGE, SLEW_RATE_CONTROL_MODE1_REG, MSKPWM_DELAY_PHASE_ENABLE);
    // Setting Driving/Sinking Channel Slew Rate
    snled27351_write_register(index, FUNCTION_PAGE, SLEW_RATE_CONTROL_MODE2_REG, MSKDRIVING_SINKING_CHHANNEL_SLEWRATE_ENABLE);
    // Setting Iref
    snled27351_write_register(index, FUNCTION_PAGE, SOFTWARE_SLEEP_REG, MSKSLEEP_DISABLE);

    // Set LED CONTROL PAGE (Page 0)
    uint8_t on_off_reg[LED_CONTROL_ON_OFF_LENGTH] = {0};
    snled27351_write(index, LED_CONTROL_PAGE, 0, on_off_reg, LED_CONTROL_ON_OFF_LENGTH);

    // Set PWM PAGE (Page 1)
    uint8_t pwm_reg[LED_PWM_LENGTH];
    memset(pwm_reg, 0, LED_PWM_LENGTH);
    snled27351_write(index, LED_PWM_PAGE, 0, pwm_reg, LED_PWM_LENGTH);

    // Set CURRENT PAGE (Page 4)
    uint8_t current_tune_reg[LED_CURRENT_TUNE_LENGTH] = SNLED27351_CURRENT_TUNE;
    snled27351_write(index, CURRENT_TUNE_PAGE, 0, current_tune_reg, LED_CURRENT_TUNE_LENGTH);

    // Setting LED driver to normal mode
    snled27351_write_register(index, FUNCTION_PAGE, CONFIGURATION_REG, MSKSW_NORMAL_MODE);
}

void snled27351_set_color(int index, uint8_t red, uint8_t green, uint8_t blue) {
    snled27351_led_t led;
    if (index >= 0 && index < SNLED27351_LED_COUNT) {
        memcpy_P(&led, (&g_snled27351_leds[index]), sizeof(led));

        g_pwm_buffer[led.driver][led.r]          = red;
        g_pwm_buffer[led.driver][led.g]          = green;
        g_pwm_buffer[led.driver][led.b]          = blue;
        g_pwm_buffer_update_required[led.driver] = true;
    }
}

void snled27351_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
    for (int i = 0; i < SNLED27351_LED_COUNT; i++) {
        snled27351_set_color(i, red, green, blue);
    }
}

void snled27351_set_led_control_register(uint8_t index, bool red, bool green, bool blue) {
    snled27351_led_t led;
    memcpy_P(&led, (&g_snled27351_leds[index]), sizeof(led));

    uint8_t control_register_r = led.r / 8;
    uint8_t control_register_g = led.g / 8;
    uint8_t control_register_b = led.b / 8;
    uint8_t bit_r              = led.r % 8;
    uint8_t bit_g              = led.g % 8;
    uint8_t bit_b              = led.b % 8;

    if (red) {
        g_led_control_registers[led.driver][control_register_r] |= (1 << bit_r);
    } else {
        g_led_control_registers[led.driver][control_register_r] &= ~(1 << bit_r);
    }
    if (green) {
        g_led_control_registers[led.driver][control_register_g] |= (1 << bit_g);
    } else {
        g_led_control_registers[led.driver][control_register_g] &= ~(1 << bit_g);
    }
    if (blue) {
        g_led_control_registers[led.driver][control_register_b] |= (1 << bit_b);
    } else {
        g_led_control_registers[led.driver][control_register_b] &= ~(1 << bit_b);
    }

    g_led_control_registers_update_required[led.driver] = true;
}

void snled27351_update_pwm_buffers(uint8_t index) {
    if (g_pwm_buffer_update_required[index]) {
        snled27351_write(index, LED_PWM_PAGE, 0, g_pwm_buffer[index], SNLED27351_PWM_REGISTER_COUNT);
        g_pwm_buffer_update_required[index] = false;
    }
}

void snled27351_update_led_control_registers(uint8_t index) {
    if (g_led_control_registers_update_required[index]) {
        snled27351_write(index, LED_CONTROL_PAGE, 0, g_led_control_registers[index], SNLED27351_LED_CONTROL_REGISTER_COUNT);
        g_led_control_registers_update_required[index] = false;
    }
}

void snled27351_flush(void) {
    for (uint8_t i = 0; i < SNLED27351_DRIVER_COUNT; i++)
        snled27351_update_pwm_buffers(i);
}

void snled27351_shutdown(void) {
#if defined(SNLED27351_SDB_PIN)
    gpio_write_pin_low(SNLED27351_SDB_PIN);
#else
    for (uint8_t i = 0; i < SNLED27351_DRIVER_COUNT; i++)
        snled27351_sw_shutdown(i);
#endif
}

void snled27351_exit_shutdown(void) {
#if defined(SNLED27351_SDB_PIN)
    gpio_write_pin_high(SNLED27351_SDB_PIN);
#else
    for (uint8_t i = 0; i < SNLED27351_DRIVER_COUNT; i++)
        snled27351_sw_return_normal(i);
#endif
}

void snled27351_sw_return_normal(uint8_t index) {
    // Setting LED driver to normal mode
    snled27351_write_register(index, FUNCTION_PAGE, CONFIGURATION_REG, MSKSW_NORMAL_MODE);
}

void snled27351_sw_shutdown(uint8_t index) {
    // Setting LED driver to shutdown mode
    snled27351_write_register(index, FUNCTION_PAGE, CONFIGURATION_REG, MSKSW_SHUT_DOWN_MODE);
    // Write SW Sleep Register
    snled27351_write_register(index, FUNCTION_PAGE, SOFTWARE_SLEEP_REG, MSKSLEEP_ENABLE);
}

// Custom RGB Matrix driver definition
#include "rgb_matrix.h"

const rgb_matrix_driver_t rgb_matrix_driver = {
    .init          = snled27351_init_drivers,
    .flush         = snled27351_flush,
    .set_color     = snled27351_set_color,
    .set_color_all = snled27351_set_color_all,
};
