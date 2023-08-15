// Wurkkos TS10 (RGB aux version) config options for Anduril
// Copyright (C) 2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// most things are the same as TS25
#include "cfg-wurkkos-ts25.h"
#undef MODEL_NUMBER
#define MODEL_NUMBER "0713"
// ATTINY: 1616

///// apply some config from the TS10 /////

// blink numbers on the aux LEDs by default
#undef DEFAULT_BLINK_CHANNEL
#define DEFAULT_BLINK_CHANNEL  CM_AUXWHT

// use the TS10 ramp, not the TS25 ramp
#undef PWM1_LEVELS
#undef PWM2_LEVELS
#undef PWM_TOPS
#define PWM1_LEVELS  1,1,2,3,3,4,5,6,7,8,9,11,12,13,15,16,18,19,21,23,26,27,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,71,74,76,78,80,82,85,87,90,93,96,100,103,107,112,116,122,127,133,140,147,154,163,171,182,192,203,215,228,241,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0
#define PWM2_LEVELS  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,5,7,9,11,13,15,17,19,21,23,25,27,29,32,34,36,39,41,43,46,49,51,54,57,59,62,65,68,71,74,77,80,83,86,90,93,96,100,103,107,110,114,117,121,125,129,133,137,141,145,149,153,157,162,166,170,175,179,184,189,193,198,203,208,213,218,223,228,233,239,244,249,255
#define PWM_TOPS     4095,2701,3200,3586,2518,2778,2834,2795,2705,2587,2455,2582,2412,2247,2256,2091,2062,1907,1860,1802,1737,1605,1542,1477,1412,1347,1284,1222,1162,1105,1050,997,946,898,853,810,768,730,693,658,625,594,564,536,503,485,462,439,418,398,384,366,353,340,327,319,307,298,292,284,280,273,269,266,263,260,258,256,256,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255

// at Wurkkos's request, reduce the Simple UI ceiling a little bit
// (i.e. not 150; original config had it at 144/150, or DD FET 204/255)
// 20 47 [75] 102 130
#undef SIMPLE_UI_FLOOR
#undef SIMPLE_UI_CEIL
#define SIMPLE_UI_FLOOR  20
#define SIMPLE_UI_CEIL   130

// the default of 26 looks a bit rough, so increase it to make it smoother
#define CANDLE_AMPLITUDE 33

// enable factory reset on 13H without loosening tailcap (required)
#define USE_SOFT_FACTORY_RESET

