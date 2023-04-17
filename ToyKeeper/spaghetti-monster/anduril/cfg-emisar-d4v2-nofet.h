// Emisar D4v2-noFET config options for Anduril
// Copyright (C) 2020-2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "cfg-emisar-d4v2.h"
#undef MODEL_NUMBER
#define MODEL_NUMBER "0115"
// ATTINY: 1634

// switch to 1-channel support functions
#undef USE_SET_LEVEL_2CH_STACKED
#undef USE_GRADUAL_TICK_2CH_STACKED
#define USE_SET_LEVEL_1CH
#define USE_GRADUAL_TICK_1CH
#undef SET_LEVEL_MODES
#undef GRADUAL_TICK_MODES
#define SET_LEVEL_MODES      set_level_1ch
#define GRADUAL_TICK_MODES   gradual_tick_1ch

#undef PWM_CHANNELS
#define PWM_CHANNELS 1

#undef LOW_PWM_LEVELS
#undef HIGH_PWM_LEVELS
#define LOW_PWM_LEVELS 1,1,1,2,2,2,2,3,3,3,3,4,4,5,5,6,6,6,7,8,8,9,9,10,10,11,12,13,13,14,15,16,16,17,18,19,20,21,22,23,23,24,26,27,28,29,30,31,32,33,34,36,37,38,39,41,42,43,45,46,47,49,50,52,53,55,56,58,59,61,62,64,66,67,69,71,72,74,76,78,80,81,83,85,87,89,91,93,95,97,99,101,103,105,107,109,111,113,116,118,120,122,125,127,129,132,134,136,139,141,144,146,148,151,154,156,159,161,164,166,169,172,174,177,180,183,185,188,191,194,197,200,203,205,208,211,214,217,220,223,226,230,233,236,239,242,245,249,252,255

#undef MAX_1x7135
#define MAX_1x7135 150
#undef QUARTERSPEED_LEVEL
#undef HALFSPEED_LEVEL
#define QUARTERSPEED_LEVEL 8
#define HALFSPEED_LEVEL 16

#define DEFAULT_LEVEL 80

#undef RAMP_SMOOTH_CEIL
#define RAMP_SMOOTH_CEIL 150
// 10, 45, 80, 115, 150
#undef RAMP_DISCRETE_FLOOR
#undef RAMP_DISCRETE_CEIL
#undef RAMP_DISCRETE_STEPS
#define RAMP_DISCRETE_FLOOR 10
#define RAMP_DISCRETE_CEIL  150
#define RAMP_DISCRETE_STEPS 5

// safe limit ~100% power because no FET
#undef SIMPLE_UI_FLOOR
#undef SIMPLE_UI_CEIL
#define SIMPLE_UI_FLOOR RAMP_DISCRETE_FLOOR
#define SIMPLE_UI_CEIL RAMP_DISCRETE_CEIL

#define CANDLE_AMPLITUDE 60

#undef THERM_FASTER_LEVEL
#define THERM_FASTER_LEVEL 150

// maybe keep this, in case someone uses a higher power channel?
//#undef USE_THERMAL_REGULATION
//#undef USE_SET_LEVEL_GRADUALLY

