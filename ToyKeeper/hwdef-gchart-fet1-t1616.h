// gChart's custom FET+1 driver layout
// Copyright (C) 2020-2023 (FIXME)
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

/*
 * PB0 - PWM for FET (TCA - WO0)
 * PB1 - PWM for 7135 (TCA - WO1)
 * PB2 - Switch pin, internal pullup
 * PB3 - Aux LED with 4700 Ohm series resistor
 * Read voltage from VCC pin, has diode with ~0.4v drop
*/


#define LAYOUT_DEFINED

#ifdef ATTINY
#undef ATTINY
#endif
#define ATTINY 1616
#include <avr/io.h>

#define PWM_CHANNELS 2

#ifndef SWITCH_PIN
#define SWITCH_PIN     PIN2_bp
#define SWITCH_PORT    VPORTB.IN
#define SWITCH_ISC_REG PORTB.PIN2CTRL
#define SWITCH_VECT    PORTB_PORT_vect
#define SWITCH_INTFLG  VPORTB.INTFLAGS
#endif


// 7135 channel
#ifndef PWM1_PIN
#define PWM1_PIN PB1               //
#define PWM1_LVL TCA0.SINGLE.CMP1  // CMP1 is the output compare register for PB1
#endif

// FET channel
#ifndef PWM2_PIN
#define PWM2_PIN PB0               //
#define PWM2_LVL TCA0.SINGLE.CMP0  // CMP0 is the output compare register for PB0
#endif

// average drop across diode on this hardware
#ifndef VOLTAGE_FUDGE_FACTOR
#define VOLTAGE_FUDGE_FACTOR 8  // 4 = add 0.20V
#endif


// lighted button
#ifndef AUXLED_PIN
#define AUXLED_PIN  PIN3_bp
#define AUXLED_PORT PORTB
#endif


// with so many pins, doing this all with #ifdefs gets awkward...
// ... so just hardcode it in each hwdef file instead
inline void hwdef_setup() {

    // set up the system clock to run at 10 MHz instead of the default 3.33 MHz
    _PROTECTED_WRITE( CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm );

    //VPORTA.DIR = 0b00000010;
    VPORTB.DIR = PIN0_bm | PIN1_bm | PIN3_bm;
    //VPORTC.DIR = 0b00000000;

    // enable pullups on the input pins to reduce power
    PORTA.PIN0CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN1CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;

    //PORTB.PIN0CTRL = PORT_PULLUPEN_bm; // FET channel
    //PORTB.PIN1CTRL = PORT_PULLUPEN_bm; // 7135 channel
    PORTB.PIN2CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;  // switch
    //PORTB.PIN3CTRL = PORT_PULLUPEN_bm; // Aux LED
    PORTB.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTB.PIN5CTRL = PORT_PULLUPEN_bm;

    PORTC.PIN0CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN1CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTC.PIN3CTRL = PORT_PULLUPEN_bm;

    // set up the PWM
    // https://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny1614-16-17-DataSheet-DS40002204A.pdf
    // PB0 is TCA0:WO0, use TCA_SINGLE_CMP0EN_bm
    // PB1 is TCA0:WO1, use TCA_SINGLE_CMP1EN_bm
    // PB2 is TCA0:WO2, use TCA_SINGLE_CMP2EN_bm
    // For Fast (Single Slope) PWM use TCA_SINGLE_WGMODE_SINGLESLOPE_gc
    // For Phase Correct (Dual Slope) PWM use TCA_SINGLE_WGMODE_DSBOTTOM_gc
    // See the manual for other pins, clocks, configs, portmux, etc
    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1EN_bm | TCA_SINGLE_WGMODE_DSBOTTOM_gc;
    TCA0.SINGLE.PER = 255;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
}

