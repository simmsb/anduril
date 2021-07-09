// Emisar D4-219C config options for Anduril
// same as D4S but with FET modes limited to 80% power
// to avoid destroying the LEDs
#include "cfg-emisar-d4.h"
#undef MODEL_NUMBER
#define MODEL_NUMBER "0112"

// don't turn off first channel at turbo level
#undef PWM1_LEVELS
#define PWM1_LEVELS 1,1,2,2,3,3,4,4,5,6,7,8,9,10,12,13,14,15,17,19,20,22,24,26,29,31,34,36,39,42,45,48,51,55,59,62,66,70,75,79,84,89,93,99,104,110,115,121,127,134,140,147,154,161,168,176,184,192,200,209,217,226,236,245,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
// 65% FET power
#undef PWM2_LEVELS
#define PWM2_LEVELS 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,13,15,16,17,18,19,21,22,23,25,26,27,28,30,32,33,34,36,38,39,41,42,44,46,47,49,51,53,55,57,59,61,63,65,67,69,71,73,75,78,80,82,84,87,90,92,94,97,99,102,104,108,110,113,116,119,121,125,127,130,134,136,140,143,146,149,153,156,159,163,166
