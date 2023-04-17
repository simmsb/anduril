/*
 * lockout-mode.c: Lockout mode for Anduril.
 *
 * Copyright (C) 2017 Selene Scriven
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#ifndef LOCKOUT_MODE_C
#define LOCKOUT_MODE_C

#include "lockout-mode.h"

uint8_t lockout_state(Event event, uint16_t arg) {
    #ifdef USE_MOON_DURING_LOCKOUT_MODE
    // momentary(ish) moon mode during lockout
    // button is being held
    #ifdef USE_AUX_RGB_LEDS
    // don't turn on during RGB aux LED configuration
    if (event == EV_click7_hold) { set_level(0); } else
    #endif
    if ((event & (B_CLICK | B_PRESS)) == (B_CLICK | B_PRESS)) {
        // hold: lowest floor
        // click, hold: highest floor (or manual mem level)
        uint8_t lvl = cfg.ramp_floors[0];
        if ((event & 0x0f) == 2) {  // second click
            if (cfg.ramp_floors[1] > lvl) lvl = cfg.ramp_floors[1];
            #ifdef USE_MANUAL_MEMORY
            if (cfg.manual_memory) lvl = cfg.manual_memory;
            #endif
        } else {  // anything except second click
            if (cfg.ramp_floors[1] < lvl) lvl = cfg.ramp_floors[1];
        }
        set_level(lvl);
    }
    // button was released
    else if ((event & (B_CLICK | B_PRESS)) == (B_CLICK)) {
        set_level(0);
    }
    #endif  // ifdef USE_MOON_DURING_LOCKOUT_MODE

    // regular event handling
    // conserve power while locked out
    // (allow staying awake long enough to exit, but otherwise
    //  be persistent about going back to sleep every few seconds
    //  even if the user keeps pressing the button)
    #ifdef USE_INDICATOR_LED
    // redundant, sleep tick does the same thing
    //if (event == EV_enter_state) {
    //    indicator_led_update(cfg.indicator_led_mode >> 2, 0);
    //} else
    #elif defined(USE_AUX_RGB_LEDS)
    if (event == EV_enter_state) {
        rgb_led_update(cfg.rgb_led_lockout_mode, 0);
    } else
    #endif

    if (event == EV_tick) {
        if (arg > HOLD_TIMEOUT) {
            go_to_standby = 1;
            #ifdef USE_INDICATOR_LED
            // redundant, sleep tick does the same thing
            //indicator_led_update(cfg.indicator_led_mode >> 2, arg);
            #elif defined(USE_AUX_RGB_LEDS)
            rgb_led_update(cfg.rgb_led_lockout_mode, arg);
            #endif
        }
        return MISCHIEF_MANAGED;
    }

    #if defined(TICK_DURING_STANDBY) && (defined(USE_INDICATOR_LED) || defined(USE_AUX_RGB_LEDS))
    else if (event == EV_sleep_tick) {
        #if defined(USE_INDICATOR_LED)
        indicator_led_update(cfg.indicator_led_mode >> 2, arg);
        #elif defined(USE_AUX_RGB_LEDS)
        rgb_led_update(cfg.rgb_led_lockout_mode, arg);
        #endif
        return MISCHIEF_MANAGED;
    }
    #endif

    // 3 clicks: exit and turn off
    else if (event == EV_3clicks) {
        blink_once();
        set_state(off_state, 0);
        return MISCHIEF_MANAGED;
    }

    // 4 clicks: exit and turn on
    else if (event == EV_4clicks) {
        #ifdef USE_MANUAL_MEMORY
        // FIXME: memory timer is totally ignored
        if (cfg.manual_memory)
            set_state(steady_state, cfg.manual_memory);
        else
        #endif
        set_state(steady_state, memorized_level);
        return MISCHIEF_MANAGED;
    }

    // 4 clicks, but hold last: exit and start at floor
    else if (event == EV_click4_hold) {
        //blink_once();
        blip();
        // reset button sequence to avoid activating anything in ramp mode
        current_event = 0;
        // ... and back to ramp mode
        set_state(steady_state, 1);
        return MISCHIEF_MANAGED;
    }

    // 5 clicks: exit and turn on at ceiling level
    else if (event == EV_5clicks) {
        set_state(steady_state, MAX_LEVEL);
        return MISCHIEF_MANAGED;
    }

    ////////// Every action below here is blocked in the (non-Extended) Simple UI //////////

    #if defined(USE_SIMPLE_UI) && !defined(USE_EXTENDED_SIMPLE_UI)
    if (cfg.simple_ui_active) {
        return EVENT_NOT_HANDLED;
    }
    #endif  // if simple UI but not extended simple UI

    #if defined(USE_INDICATOR_LED)
    // 7 clicks: rotate through indicator LED modes (lockout mode)
    else if (event == EV_7clicks) {
        #if defined(USE_INDICATOR_LED)
            uint8_t mode = cfg.indicator_led_mode >> 2;
            #ifdef TICK_DURING_STANDBY
            mode = (mode + 1) & 3;
            #else
            mode = (mode + 1) % 3;
            #endif
            #ifdef INDICATOR_LED_SKIP_LOW
            if (mode == 1) { mode ++; }
            #endif
            cfg.indicator_led_mode = (mode << 2) + (cfg.indicator_led_mode & 0x03);
            // redundant, sleep tick does the same thing
            //indicator_led_update(cfg.indicator_led_mode >> 2, arg);
        #elif defined(USE_AUX_RGB_LEDS)
        #endif
        save_config();
        return MISCHIEF_MANAGED;
    }
    #elif defined(USE_AUX_RGB_LEDS)
    // 7 clicks: change RGB aux LED pattern
    else if (event == EV_7clicks) {
        uint8_t mode = (cfg.rgb_led_lockout_mode >> 4) + 1;
        mode = mode % RGB_LED_NUM_PATTERNS;
        cfg.rgb_led_lockout_mode = (mode << 4) | (cfg.rgb_led_lockout_mode & 0x0f);
        rgb_led_update(cfg.rgb_led_lockout_mode, 0);
        save_config();
        blink_once();
        return MISCHIEF_MANAGED;
    }
    // 7H: change RGB aux LED color
    else if (event == EV_click7_hold) {
        setting_rgb_mode_now = 1;
        if (0 == (arg & 0x3f)) {
            uint8_t mode = (cfg.rgb_led_lockout_mode & 0x0f) + 1;
            mode = mode % RGB_LED_NUM_COLORS;
            cfg.rgb_led_lockout_mode = mode | (cfg.rgb_led_lockout_mode & 0xf0);
            //save_config();
        }
        rgb_led_update(cfg.rgb_led_lockout_mode, arg);
        return MISCHIEF_MANAGED;
    }
    // 7H, release: save new color
    else if (event == EV_click7_hold_release) {
        setting_rgb_mode_now = 0;
        save_config();
        return MISCHIEF_MANAGED;
    }
    #endif

    #if defined(USE_EXTENDED_SIMPLE_UI) && defined(USE_SIMPLE_UI)
    ////////// Every action below here is blocked in the Extended Simple UI //////////
    if (cfg.simple_ui_active) {
        return EVENT_NOT_HANDLED;
    }
    #endif  // if extended simple UI

    #ifdef USE_AUTOLOCK
    // 10H: configure the autolock option
    else if (event == EV_click10_hold) {
        push_state(autolock_config_state, 0);
        return MISCHIEF_MANAGED;
    }
    #endif

    return EVENT_NOT_HANDLED;
}

#ifdef USE_AUTOLOCK
// set the auto-lock timer to N minutes, where N is the number of clicks
void autolock_config_save(uint8_t step, uint8_t value) {
    cfg.autolock_time = value;
}

uint8_t autolock_config_state(Event event, uint16_t arg) {
    return config_state_base(event, arg, 1, autolock_config_save);
}
#endif  // #ifdef USE_AUTOLOCK


#endif

