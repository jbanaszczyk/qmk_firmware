#include <quantum.h>
#include "config.h"
#include "alt_layers.h"

static layer_state_t last_layer_state = 1;

layer_state_t layer_state_set_alt_layers(layer_state_t state) {
    layer_state_t previous_layer_state = layer_state;
    if (previous_layer_state == 0) {
        previous_layer_state = 1;
    }
    if (state == 0) {
        state = 1;
    }

    dprintf("\n????? %lX %lX\n", previous_layer_state, state);

//    if (previous_layer_state != state) {
        uint8_t layer_modified = get_highest_layer(previous_layer_state ^ state);
        bool layer_on = layer_state_cmp(state, layer_modified);
        if (layer_on) {
            dprint("+++++\n");
            last_layer_state = previous_layer_state;
            state = 1UL << layer_modified;
        } else {
            dprint("-----\n");
            state = last_layer_state;
        }
//    }
    return state;
}

/*
static bool process_record_test(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_TEST) {
        if (record->event.pressed) {
            dprint("******\n");
        }
        return false;
    }
    return true;
}

typedef struct {
    keyevent_t event;
#ifndef NO_ACTION_TAPPING
    tap_t tap;
#endif
} keyrecord_t;

typedef struct {
    bool    interrupted : 1;
    bool    reserved2 : 1;
    bool    reserved1 : 1;
    bool    reserved0 : 1;
    uint8_t count : 4;
} tap_t;

typedef struct {
    keypos_t key;
    bool     pressed;
    uint16_t time;
} keyevent_t;
*/


bool process_record_alt_layers(uint16_t keycode, keyrecord_t *record) {

    switch (keycode) {

        case QK_LAYER_TAP_TOGGLE_MAX - 1 ... QK_LAYER_TAP_TOGGLE_MAX: {

            int layer = keycode & 0xFF;

            dprintf("QK_LAYER_TAP_TOGGLE %d %s %d\n", layer, record->event.pressed ? "+" : "-", record->tap.count);

            if (record->event.pressed) {

                if (record->tap.count == 0) {
                    last_layer_state = layer_state;
                    layer_move(layer);
                    return false;
                }


            } else {

                if (record->tap.count == 0) {
                    layer_state_set(last_layer_state);
                    last_layer_state = layer_state;
                    return false;
                }

                if (record->tap.count == TAPPING_TOGGLE) {
                    last_layer_state = layer_state;
                    layer_move(layer);
                    return false;
                }
                if (record->tap.count == 1) {
                    layer_on(layer);
                    set_oneshot_layer(layer, ONESHOT_START);
                    return false;
                }

            }
            return false;


            break;
        }
    }

    return true;
}
