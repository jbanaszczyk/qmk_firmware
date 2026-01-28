# Migration Guide: K1 Pro Minus to Modern QMK Architecture

Based on analysis of both `k1_pro_minus` (old architecture) and `k1_version_6` (modern architecture), here's a
comprehensive migration guide.

---

## Current State Analysis

### Key Differences Found:

| Aspect         | k1_pro_minus (Old)                          | k1_version_6 (Modern)                                |
|----------------|---------------------------------------------|------------------------------------------------------|
| **rules.mk**   | Local `SRC += matrix.c`, `VIA_ENABLE = yes` | Includes `keychron_common.mk`, sets `VPATH`          |
| **Matrix**     | Local `matrix.c` with hardcoded HC595 pins  | Uses shared `common/matrix.c` with configurable pins |
| **Keyboard C** | Uses `matrix_scan_kb()` for tasks           | Uses `keychron_task_kb()` hook                       |
| **Keycodes**   | Defines own keycodes in `.h` file           | Uses `keychron_common.h` keycodes                    |

---

## Step 1: Update `rules.mk`

**Current `k1_pro_minus/rules.mk`:**

```make
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
OPT_DEFS += -DNO_USB_STARTUP_CHECK

VIA_ENABLE = yes

SRC += matrix.c
```

**Migrated version:**

```make
# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE
OPT_DEFS += -DNO_USB_STARTUP_CHECK

# Include common Keychron files
include keyboards/keychron/common/keychron_common.mk

VPATH += $(TOP_DIR)/keyboards/keychron

# Custom matrix with 74HC595 shift register
SRC += common/matrix.c
```

**Changes:**

- Remove `VIA_ENABLE = yes` (move to `info.json` as `"via": true` under features, or keep if preferred)
- Remove local `SRC += matrix.c` and add `SRC += common/matrix.c` to use the shared matrix implementation
- Add `include keyboards/keychron/common/keychron_common.mk`
- Add `VPATH += $(TOP_DIR)/keyboards/keychron`

**Important:** The shared `common/matrix.c` is NOT included by `keychron_common.mk` — it must be explicitly added via `SRC += common/matrix.c`.

---

## Step 2: Update `info.json`

**Add these features:**

```json
{
  "features": {
    "bootmagic": true,
    "mousekey": true,
    "extrakey": true,
    "nkro": true,
    "raw": true,
    "via": true
  }
}
```

**Note:** The current `info.json` already has:

- Correct `matrix_pins` with `"custom": true` and `"custom_lite": true` for shift-register scanning
- Correct `matrix_size` (6 rows × 17 cols)
- Correct layouts (`LAYOUT_tkl_ansi`, `LAYOUT_tkl_iso`) that map to the physical matrix

**Remove:** `"dip_switch": true` from features — DIP switch has no functionality after migration (no `dip_switch_update_kb` handler).

---

## Step 3: Update `config.h`

**Add HC595 shift register pin definitions** (required for shared `common/matrix.c`):

```c
// Shift register pins for matrix scanning (74HC595)
#define HC595_STCP B0
#define HC595_SHCP A1
#define HC595_DS A7

// Shift register column range (columns 1-16 are via shift register, column 0 is direct)
#define HC595_START_INDEX 1
#define HC595_END_INDEX 16
```

**Keep existing hardware-specific macros:**

- I2C timing registers (`I2C1_TIMINGR_*`)
- EEPROM/wear-leveling settings
- `LED_CAPS_LOCK_PIN`, `LED_PIN_ON_STATE`

**Remove:**

- `DIP_SWITCH_PINS` — DIP switch has no functionality after migration
- `FN_KEY1`, `FN_KEY2` — Factory test keys are not needed (factory test is optional and these are only used there)

---

## Step 4: Delete Local `matrix.c`

Delete `k1_pro_minus/matrix.c` entirely. The shared `keyboards/keychron/common/matrix.c` will be used automatically via
the `VPATH` and handles:

- 74HC595 shift register logic
- Configurable pin definitions via `HC595_STCP`, `HC595_SHCP`, `HC595_DS`
- Configurable column range via `HC595_START_INDEX`, `HC595_END_INDEX`

---

## Step 5: Update `k1_pro_minus.h`

**Current version** defines its own keycodes. **Migrated version** should use `keychron_common.h`:

```c
#pragma once

#include "quantum.h"
#include "keychron_common.h"

#ifdef VIA_ENABLE
#    include "via.h"
#endif

#define ___ KC_NO
```

**Remove** the entire `enum` block that defines `KC_LOPTN`, `KC_SIRI`, etc. — these are now provided by
`keychron_common.h`.

---

## Step 6: Update `k1_pro_minus.c`

**Current version** uses `matrix_scan_kb()` for periodic tasks. **Migrated version** should use `keychron_task_kb()`:

```c
#include "quantum.h"
#include "keychron_task.h"

#define POWER_ON_LED_DURATION 3000
static uint32_t power_on_indicator_timer = 0;

void keyboard_post_init_kb(void) {
    power_on_indicator_timer = timer_read32();
    gpio_write_pin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
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
    return true;
}

void suspend_wakeup_init_kb(void) {
    clear_keyboard();
    send_keyboard_report();
}
```

**Key changes:**

- Remove `#include "k1_pro_minus.h"` → use `#include "quantum.h"` and `#include "keychron_task.h"`
- Remove `process_record_kb()` for `KC_SIRI` handling — now handled by `keychron_common.c`
- Remove `matrix_scan_kb()` → use `keychron_task_kb()` instead
- Remove `via_command_kb()` and `raw_hid_receive()` — now handled by `keychron_common.c`
- Remove local `key_combination_t` and `key_comb_list` — provided by common code
- **Remove `dip_switch_update_kb()`** — intentionally deleted per requirements

---

## Step 7: Verify Layout Mapping

The current `info.json` layouts are **correctly mapped** to the physical matrix:

- **LAYOUT_tkl_ansi**: 87 keys for ANSI TKL
- **LAYOUT_tkl_iso**: 88 keys for ISO TKL

Matrix positions like `{"matrix":[0,0], "x":0, "y":0}` correctly reference the 6×17 matrix with the shift-register
scanning logic.

---

## Summary of Files to Modify

| File             | Action                                                        |
|------------------|---------------------------------------------------------------|
| `rules.mk`       | Update to include common files, remove `SRC += matrix.c`      |
| `info.json`      | Add `"via": true`, remove `"dip_switch": true`                |
| `config.h`       | Add `HC595_*` defines, remove `DIP_SWITCH_PINS`, `FN_KEY1/2`  |
| `matrix.c`       | **DELETE** (use shared common/matrix.c)                       |
| `k1_pro_minus.h` | Replace keycode enum with `#include "keychron_common.h"`      |
| `k1_pro_minus.c` | Refactor to use `keychron_task_kb()`, remove duplicated logic |

---

## Build Command

After migration, build with:

```bash
make keychron/k1_pro_minus/ansi/rgb:default
make keychron/k1_pro_minus/ansi/rgb:via
```

The modern architecture provides:

- Shared keycode handling (`KC_SIRI`, `KC_TASK`, etc.)
- Shared matrix scanning for 74HC595
- Factory test support
- Cleaner, more maintainable code structure
