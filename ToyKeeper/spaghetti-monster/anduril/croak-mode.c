#pragma once

#include "croak-mode.h"

uint8_t croak_state(Event event, uint16_t arg) {
    if (event == EV_1click) {
        set_state(off_state, 0);
        return EVENT_HANDLED;
    }

    return EVENT_NOT_HANDLED;
}

struct morse {
    uint8_t len;
    uint8_t data;
};

struct morse get_morse(char c) {
    switch (c) {
        case 'c':
            return (struct morse){.len = 4, .data = 0b1010};
        case 'r':
            return (struct morse){.len = 3, .data = 0b010};
        case 'o':
            return (struct morse){.len = 3, .data = 0b111};
        case 'a':
            return (struct morse){.len = 2, .data = 0b01};
        case 'k':
            return (struct morse){.len = 3, .data = 0b101};
        case '!':
            return (struct morse){.len = 6, .data = 0b101011};
        default:
            return (struct morse){.len = 0, .data = 0};
    }
}

void blink_morse(char c) {
    struct morse code = get_morse(c);

    #define DIT_LENGTH 200

    for (int i = 0; i < code.len; i++) {
        int dash = (code.data >> (code.len - i - 1)) & 0b1;
        set_level(memorized_level);
        if (dash) {
            nice_delay_ms(DIT_LENGTH * 3);
        } else {
            nice_delay_ms(DIT_LENGTH);
        }
        set_level(0);
        nice_delay_ms(DIT_LENGTH);
    }
    nice_delay_ms(DIT_LENGTH * 2);
}

inline void croak_mode_iter() {
    blink_morse('c');
    blink_morse('r');
    blink_morse('o');
    blink_morse('a');
    blink_morse('k');
    blink_morse('!');
    nice_delay_ms(2000);
}
