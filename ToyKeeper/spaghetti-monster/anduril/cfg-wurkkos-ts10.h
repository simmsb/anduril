// Wurkkos TS10 (originally used Sofirn SP36-t1616 firmware) config options for Anduril using the Attiny1616
// same as  the  BLF Q8 T1616, mostly (added Dynamic PWM)
#define MODEL_NUMBER "0714"
#include "hwdef-Wurkkos_TS10.h"
// ATTINY: 1616

// uses forward-facing aux LEDs
#define USE_INDICATOR_LED
// don't turn on the aux LEDs while main LEDs are on
#ifdef USE_INDICATOR_LED_WHILE_RAMPING
#undef USE_INDICATOR_LED_WHILE_RAMPING
#endif
// the high button LED mode on this light uses too much power, default to low
// off mode: low (1)
// lockout: blinking (3)
#define INDICATOR_LED_DEFAULT_MODE ((3<<2) + 1)

// voltage readings were a little high with the Q8 value
#undef VOLTAGE_FUDGE_FACTOR
#define VOLTAGE_FUDGE_FACTOR 5  // add 0.25V, not 0.35V

/*
// copied from Emisar D4 ramp
// ../../bin/level_calc.py 1 65 7135 1 0.8 150
// ... mixed with this:
// ../../bin/level_calc.py 2 150 7135 4 0.33 150 FET 1 10 1500
#define RAMP_LENGTH 150
#define PWM1_LEVELS 1,1,2,2,3,3,4,4,5,6,7,8,9,10,12,13,14,15,17,19,20,22,24,26,29,31,34,36,39,42,45,48,51,55,59,62,66,70,75,79,84,89,93,99,104,110,115,121,127,134,140,147,154,161,168,176,184,192,200,209,217,226,236,245,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0
#define PWM2_LEVELS 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,4,5,7,8,9,11,12,14,15,17,19,20,22,24,25,27,29,31,33,35,37,39,41,43,45,48,50,52,55,57,59,62,64,67,70,72,75,78,81,84,87,90,93,96,99,102,105,109,112,115,119,122,126,129,133,137,141,144,148,152,156,160,165,169,173,177,182,186,191,195,200,205,209,214,219,224,229,234,239,244,250,255
#define MAX_1x7135 65
#define HALFSPEED_LEVEL 14
#define QUARTERSPEED_LEVEL 5
*/
// level 1 by hand, for the rest
// level_calc.py 7.01 2 149 7135 3 0.5 125 FET 1 10 1200 --pwm dyn:63:2048:255
#define RAMP_LENGTH 150
#define USE_DYN_PWM
#define PWM1_LEVELS 1,3,3,4,5,6,7,8,9,10,12,13,14,16,17,19,20,22,24,25,27,29,31,33,35,37,40,42,44,47,49,52,54,57,59,62,64,67,70,72,75,77,80,82,85,87,89,91,93,95,96,98,99,100,100,101,100,100,99,97,95,93,90,86,82,87,91,96,100,106,111,116,122,128,134,141,147,155,162,169,177,186,194,203,213,222,232,243,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0
#define PWM2_LEVELS 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,4,6,7,9,11,12,14,16,18,20,22,24,27,29,31,34,37,39,42,45,48,51,54,57,61,64,68,72,75,79,83,88,92,97,101,106,111,116,121,126,132,138,144,150,156,162,169,176,183,190,197,205,213,221,229,237,246,255
#define PWM_TOPS 2047,2047,1198,1322,1584,1676,1701,1691,1662,1622,1774,1703,1631,1692,1613,1639,1558,1564,1559,1478,1464,1444,1420,1392,1361,1329,1331,1293,1256,1246,1207,1192,1152,1133,1094,1074,1035,1013,991,954,932,897,875,842,820,790,760,731,704,678,646,622,593,566,534,510,478,452,423,393,364,338,310,280,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
#define MAX_1x7135 90
#define HALFSPEED_LEVEL 2
#define QUARTERSPEED_LEVEL 2

#define RAMP_SMOOTH_FLOOR 1
#define RAMP_SMOOTH_CEIL 120
// 10 28 46 [65] 83 101 120
#define RAMP_DISCRETE_FLOOR 10
#define RAMP_DISCRETE_CEIL RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 7

// at Wurkkos's request, reduce the Simple UI ceiling a little bit
#define SIMPLE_UI_FLOOR RAMP_DISCRETE_FLOOR
#define SIMPLE_UI_CEIL 144
#define SIMPLE_UI_STEPS 5

// enable 2 click turbo (Anduril 1 style)
#define DEFAULT_2C_STYLE 1

// enable SOS in the blinkies group
#define USE_SOS_MODE
#define USE_SOS_MODE_IN_BLINKY_GROUP

// Allow 3C in Simple UI for switching between smooth and stepped ramping
#define USE_SIMPLE_UI_RAMPING_TOGGLE

// allow Aux Config and Strobe Modes in Simple UI
#define USE_EXTENDED_SIMPLE_UI

// enable factory reset on 13H without loosening tailcap
#define USE_SOFT_FACTORY_RESET

// stop panicking at ~55% power
#define THERM_FASTER_LEVEL 130  // throttle back faster when high

// don't blink during the ramp or at the ceiling
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif
#ifdef BLINK_AT_RAMP_CEIL
#undef BLINK_AT_RAMP_CEIL
#endif