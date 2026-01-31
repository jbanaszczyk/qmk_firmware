# Keychron K1 Version 6 Minus

![Keychron K1 Version 6 Minus](https://cdn.shopify.com/s/files/1/0059/0630/1017/files/K1_Version_6_6.jpg)

A customizable TKL keyboard.

* Keyboard Maintainer: [jbanaszczyk](https://github.com/jbanaszczyk)
* Hardware Supported: Keychron K1 Version 6 Minus
* Hardware
  Availability: [Keychron K1 Version 6 Wireless Mechanical Keyboard](https://www.keychron.com/products/keychron-k1-wireless-mechanical-keyboard)

Make example for this keyboard (after setting up your build environment):

    make keychron/k1_version_6_minus/ansi/rgb:default
    make keychron/k1_version_6_minus/ansi/rgb:via

Flashing example for this keyboard:

    make keychron/k1_version_6_minus/ansi/rgb:default:flash
    make keychron/k1_version_6_minus/ansi/rgb:via:flash

**Reset Key**: Hold down the *Esc* key or reset button underneath space bar while connecting the USB cable.

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and
the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start
with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

### Pinout Information

#### Processor

* **MCU:** `WB32F3G71` (LQFP64)

#### Hardware Pins

* **Matrix Rows:** `C12`, `D2`, `B3`, `B4`, `B5`, `B6`
* **Matrix Columns:** `C6`, `C7`, `C8`, `A14`, `A15`, `C10`, `C11`, `C13`, `C14`, `C15`, `C0`, `C1`, `C2`, `C3`, `A0`,
  `A1`, `A2`
* **RGB Matrix SPI:** `A5` (SCK), `A6` (MISO), `A7` (MOSI)
* **RGB Driver Control:** `B7` (SDB), `A8` (CS1), `C9` (CS2)
* **EEPROM (I2C1):** `B8` (SCL), `B9` (SDA)
* **DIP Switch (Win/Mac):** `B14`
* **DIP Switch (USB/BT):** `A10` (from legacy firmware)
* **Miscellaneous:** `B2` (Hardware Revision / System)
* **Wireless Peripheral Control:**
    * `C4`: BT Reset
    * `B1`: BT INT Input
    * `A4`: BT INT Output
    * `B0`: USB Power Sense
    * `B13`: Battery Charging
    * `C5`: DP Pullup Control
* **USB:** `A11`, `A12` (Fixed system pins)
* **Something:** `D0`, `D1`

#### LED Indicator Pins
* **Caps Lock:** `A13` (active high)
* **BT Indicator (Blue):** `B15` (active low)
* **Power (Green):** `A3` (active low)

#### Unused Pins (WB32F3G71 LQFP64)
* **Battery Low (Red):** `B11` (Non-functional)
* **Other:** `A9`, `B10`, `B12`

