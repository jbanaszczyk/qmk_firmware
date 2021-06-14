#pragma once

#include "jbanaszczyk_keycodes.h"

#define DO_NOT_SLEEP_TIMEOUT 60U * 5U

//#define DO_NOT_SLEEP_ACTION_KEY KC_LCTL
#define DO_NOT_SLEEP_ACTION_KEY KC_WAKE

//#define DO_NOT_SLEEP_START_LOCKED

#define DO_NOT_SLEEP_KC_START  KC_DO_NOT_SLEEP_START
#define DO_NOT_SLEEP_KC_STOP   KC_DO_NOT_SLEEP_STOP
#define DO_NOT_SLEEP_KC_TOGGLE KC_DO_NOT_SLEEP_TOGGLE

bool    is_do_not_sleep_enabled(void);
bool    is_do_not_sleep_locked(void);
bool    do_not_sleep_enable(void);
bool    do_not_sleep_disable(void);
void    do_not_sleep_lock(void);
void    do_not_sleep_unlock(void);
void    do_not_sleep_scan(void);
bool    do_not_sleep_process(uint16_t keycode, keyrecord_t *record);
void    do_not_sleep_action(void); // weak, can be redefined
uint8_t do_not_sleep_serialize(void);
void    do_not_sleep_deserialize(uint8_t value);
