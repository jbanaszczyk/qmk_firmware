
#include <quantum.h>
#include "config.h"
#include "do_not_sleep.h"

#ifndef DO_NOT_SLEEP_TIME
#define DO_NOT_SLEEP_TIME 300UL
#endif

#ifndef DO_NOT_SLEEP_ACTION_KEY
#define DO_NOT_SLEEP_ACTION_KEY KC_WAKE
#endif

static enum DO_NOT_SLEEP_STATE {
    DO_NOT_SLEEP_ENABLED,
    DO_NOT_SLEEP_DISABLED,
    DO_NOT_SLEEP_LOCKED
} do_not_sleep_enabled = DO_NOT_SLEEP_DISABLED;

static uint32_t do_not_sleep_timer = 0;

bool is_do_not_sleep_enabled(void) {
    return do_not_sleep_enabled == DO_NOT_SLEEP_ENABLED;
}

bool is_do_not_sleep_locked(void) {
    return do_not_sleep_enabled == DO_NOT_SLEEP_LOCKED;
}

static inline void do_not_sleep_start_timer(void) {
    do_not_sleep_timer = timer_read32();
}

void do_not_sleep_unlock(void) {
    do_not_sleep_enabled = DO_NOT_SLEEP_ENABLED;
    do_not_sleep_start_timer();
}

bool do_not_sleep_disable(void) {
    if (do_not_sleep_enabled == DO_NOT_SLEEP_LOCKED) {
        return false;
    }
    do_not_sleep_enabled = DO_NOT_SLEEP_DISABLED;
    return true;
}

bool do_not_sleep_enable(void) {
    if (do_not_sleep_enabled == DO_NOT_SLEEP_LOCKED) {
        return false;
    }
    do_not_sleep_unlock();
    return true;
}

void do_not_sleep_lock(void) {
    do_not_sleep_enabled = DO_NOT_SLEEP_LOCKED;
}

__attribute__ ((weak)) void do_not_sleep_action(void) {
    dprint("Do not sleep.\n");
    tap_code(DO_NOT_SLEEP_ACTION_KEY);
}

void do_not_sleep_scan(void) {
    if (is_do_not_sleep_enabled()) {
        uint32_t do_not_sleep_delay = timer_elapsed32(do_not_sleep_timer);
        if (do_not_sleep_delay >= 1000UL * (unsigned long int) (DO_NOT_SLEEP_TIMEOUT)) {
            do_not_sleep_start_timer();
            do_not_sleep_action();
        }
    }
}

bool do_not_sleep_process(uint16_t keycode, keyrecord_t *record) {
    if (is_do_not_sleep_enabled()) {
        do_not_sleep_start_timer();
    }

#if (defined DO_NOT_SLEEP_KC_START) || (defined DO_NOT_SLEEP_KC_STOP) || (defined DO_NOT_SLEEP_KC_TOGGLE)
    if (record->event.pressed) {
        switch (keycode) {

#ifdef DO_NOT_SLEEP_KC_START
            case DO_NOT_SLEEP_KC_START: {
                do_not_sleep_unlock();
                return false;
            }
#endif

#ifdef DO_NOT_SLEEP_KC_STOP
            case DO_NOT_SLEEP_KC_STOP: {
                do_not_sleep_lock();
                return false;
            }
#endif

#ifdef DO_NOT_SLEEP_KC_TOGGLE
            case DO_NOT_SLEEP_KC_TOGGLE: {
                is_do_not_sleep_locked() ? do_not_sleep_unlock() : do_not_sleep_lock();
                return false;
            }
#endif

            default: {
                break;
            }
        }
    } else {
        switch (keycode) {

#ifdef DO_NOT_SLEEP_KC_START
            case DO_NOT_SLEEP_KC_START:
#endif

#ifdef DO_NOT_SLEEP_KC_STOP
            case DO_NOT_SLEEP_KC_STOP:
#endif

#ifdef DO_NOT_SLEEP_KC_TOGGLE
            case DO_NOT_SLEEP_KC_TOGGLE:
#endif

                return false;

            default: {
                break;
            }
        }
    }
#endif
    return true;
}

uint8_t do_not_sleep_serialize(void) {
    return (uint8_t) do_not_sleep_enabled;
}

void do_not_sleep_deserialize(uint8_t value) {
    do_not_sleep_enabled = (enum DO_NOT_SLEEP_STATE) (value);
    if (is_do_not_sleep_enabled()) {
        do_not_sleep_start_timer();
    }
}
