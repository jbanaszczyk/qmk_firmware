#include <quantum.h>
#include "do_not_sleep.h"
#include "alt_layers.h"

static bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    return true;
}

static bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
    return true;
}

static bool process_record_debug(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
    if (record->event.pressed) {
        dprintf("KL: kc: %u\n", keycode);
    }
#endif
    return true;
}

static bool process_record_alt_digit(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_1:
        case KC_2:
        case KC_3:
        case KC_4:
        case KC_5:
        case KC_6:
        case KC_7:
        case KC_8:
        case KC_9:
        case KC_0: {
            //            int mods  = get_mods();
            //            int mods1 = get_oneshot_mods();
            //            uprintf("KL: kc: %u, pressd: %u, mods: %u, one_shoot_mods: %u\n", keycode, record->event.pressed, mods, mods1);
            break;
        }
        default: {
            break;
        }
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {


    return
            true

            && process_record_alt_layers(keycode, record)

            && process_record_debug(keycode, record)

            #ifdef DO_NOT_SLEEP
            && do_not_sleep_process(keycode, record)
            #endif

            && process_record_alt_digit(keycode, record)

            && process_record_keymap(keycode, record)

            && process_record_secrets(keycode, record);

    //  switch (keycode) {
    //  case KC_QWERTY ... KC_UNICODE:
    //    if (record->event.pressed) {
    //      set_single_persistent_default_layer(keycode - KC_QWERTY);
    //    }
    //    break;
    //
    //    }
    //    }
}


