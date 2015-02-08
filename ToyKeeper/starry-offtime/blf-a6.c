/*
 * NANJG 105C Diagram
 *           ---
 *         -|   |- VCC
 *  Star 4 -|   |- Voltage ADC
 *  Star 3 -|   |- PWM
 *     GND -|   |- Star 2
 *           ---
 *
 * FUSES
 *      I use these fuse settings
 *      Low:  0x75  (4.8MHz CPU without 8x divider, 9.4kHz phase-correct PWM or 18.75kHz fast-PWM)
 *      High: 0xff
 *
 *      For more details on these settings, visit http://github.com/JCapSolutions/blf-firmware/wiki/PWM-Frequency
 *
 * STARS
 *      Star 2 = second PWM output channel
 *      Star 3 = mode memory if soldered, no memory by default
 *      Star 4 = Capacitor for off-time
 *
 * VOLTAGE
 *      Resistor values for voltage divider (reference BLF-VLD README for more info)
 *      Reference voltage can be anywhere from 1.0 to 1.2, so this cannot be all that accurate
 *
 *           VCC
 *            |
 *           Vd (~.25 v drop from protection diode)
 *            |
 *          1912 (R1 19,100 ohms)
 *            |
 *            |---- PB2 from MCU
 *            |
 *          4701 (R2 4,700 ohms)
 *            |
 *           GND
 *
 *      ADC = ((V_bat - V_diode) * R2   * 255) / ((R1    + R2  ) * V_ref)
 *      125 = ((3.0   - .25    ) * 4700 * 255) / ((19100 + 4700) * 1.1  )
 *      121 = ((2.9   - .25    ) * 4700 * 255) / ((19100 + 4700) * 1.1  )
 *
 *      Well 125 and 121 were too close, so it shut off right after lowering to low mode, so I went with
 *      130 and 120
 *
 *      To find out what value to use, plug in the target voltage (V) to this equation
 *          value = (V * 4700 * 255) / (23800 * 1.1)
 *
 */
#define F_CPU 4800000UL

/*
 * =========================================================================
 * Settings to modify per driver
 */

//#define FAST 0x23           // fast PWM channel 1 only
//#define PHASE 0x21          // phase-correct PWM channel 1 only
#define FAST 0xA3             // fast PWM both channels
#define PHASE 0xA1            // phase-correct PWM both channels

#define VOLTAGE_MON         // Comment out to disable
#define OWN_DELAY           // Should we use the built-in delay or our own?
// Adjust the timing per-driver, since the hardware has high variance
// Higher values will run slower, lower values run faster.
#define DELAY_TWEAK         950

#define OFFTIM3             // Use short/med/long off-time presses
                            // instead of just short/long

// PWM levels for the big circuit (FET or Nx7135)
#define NUM_MODES1          7
#define NUM_MODES2          4
#define MODESNx1            0,0,0,38,83,150,255
#define MODESNx2            0,0,90,255
// PWM levels for the small circuit (1x7135)
// (if the big circuit is a FET, use 0 for high modes here instead of 255)
#define MODES1x1            3,20,128,0,0,0,255
#define MODES1x2            20,160,0,255
#define MODES_PWM1          PHASE,FAST,FAST,FAST,FAST,FAST,PHASE
#define MODES_PWM2          FAST,FAST,FAST,PHASE
// Hidden modes are *before* the lowest (moon) mode, and should be specified
// in reverse order.  So, to go backward from moon to turbo to strobe to
// battcheck, use BATTCHECK,STROBE,TURBO .
#define NUM_HIDDEN          3
#define HIDDENMODES         BATTCHECK,STROBE,TURBO
#define HIDDENMODES_PWM     PHASE,PHASE,PHASE

//#define NON_WDT_TURBO            // enable turbo step-down without WDT
// How many timer ticks before before dropping down.
// Each timer tick is 500ms, so "60" would be a 30-second stepdown.
// Max value of 255 unless you change "ticks"
#define TURBO_TIMEOUT       60
                                // variable to uint8_t
//#define TURBO_RAMP_DOWN           // By default we will start to gradually ramp down, once TURBO_TIMEOUT ticks are reached, 1 PWM_LVL each tick until reaching MODE_TURBO_LOW PWM_LVL
                                // If commented out, we will step down to MODE_TURBO_LOW once TURBO_TIMEOUT ticks are reached

// These values were measured using wight's "A17HYBRID-S" driver built by DBCstm.
// Your mileage may vary.
#define ADC_42          174 // the ADC value we expect for 4.20 volts
#define ADC_100         174 // the ADC value for 100% full (4.2V resting)
#define ADC_75          165 // the ADC value for 75% full (4.0V resting)
#define ADC_50          155 // the ADC value for 50% full (3.8V resting)
#define ADC_25          141 // the ADC value for 25% full (3.5V resting)
#define ADC_0           118 // the ADC value for 0% full (3.0V resting)
#define ADC_LOW         109 // When do we start ramping down (2.8V)
#define ADC_CRIT        104 // When do we shut the light off (2.7V)
// Values for testing only:
//#define ADC_LOW         125 // When do we start ramping down (2.8V)
//#define ADC_CRIT        124 // When do we shut the light off (2.7V)

// the BLF EE A6 driver may have different offtime cap values than most other drivers
#ifdef OFFTIM3
#define CAP_SHORT           250  // Value between 1 and 255 corresponding with cap voltage (0 - 1.1v) where we consider it a short press to move to the next mode
#define CAP_MED             190  // Value between 1 and 255 corresponding with cap voltage (0 - 1.1v) where we consider it a short press to move to the next mode
#else
#define CAP_SHORT           190  // Value between 1 and 255 corresponding with cap voltage (0 - 1.1v) where we consider it a short press to move to the next mode
                                 // Not sure the lowest you can go before getting bad readings, but with a value of 70 and a 1uF cap, it seemed to switch sometimes
                                 // even when waiting 10 seconds between presses.
#endif

#define TURBO     255       // Convenience code for turbo mode
#define STROBE    254       // Convenience code for strobe mode
#define BATTCHECK 253       // Convenience code for battery check mode

/*
 * =========================================================================
 */

#ifdef OWN_DELAY
#include <util/delay_basic.h>
// Having own _delay_ms() saves some bytes AND adds possibility to use variables as input
static void _delay_ms(uint16_t n)
{
    // TODO: make this take tenths of a ms instead of ms,
    // for more precise timing?
    while(n-- > 0) _delay_loop_2(DELAY_TWEAK);
}
#else
#include <util/delay.h>
#endif

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
//#include <avr/power.h>

#define STAR2_PIN   PB0
#define STAR3_PIN   PB4
#define CAP_PIN     PB3
#define CAP_CHANNEL 0x03    // MUX 03 corresponds with PB3 (Star 4)
#define CAP_DIDR    ADC3D   // Digital input disable bit corresponding with PB3
#define PWM_PIN     PB1
#define ALT_PWM_PIN PB0
#define VOLTAGE_PIN PB2
#define ADC_CHANNEL 0x01    // MUX 01 corresponds with PB2
#define ADC_DIDR    ADC1D   // Digital input disable bit corresponding with PB2
#define ADC_PRSCL   0x06    // clk/64

#define PWM_LVL     OCR0B   // OCR0B is the output compare register for PB1
#define ALT_PWM_LVL OCR0A   // OCR0A is the output compare register for PB0

/*
 * global variables
 */

// Mode storage
uint8_t eepos = 0;
uint8_t memory = 0;
uint8_t modegroup = 0;

uint8_t mode_idx = 0;
uint8_t fast_presses = 0;
// NOTE: Only '1' is known to work; -1 will probably break and is untested.
// In other words, short press goes to the next (higher) mode,
// medium press goes to the previous (lower) mode.
#define mode_dir 1
// this is set based on the actual number of solid modes,
// not the length of the array
// also, it tracks the number of hidden modes...
uint8_t mode_cnt;
uint8_t solid_modes;
//uint8_t hidden_modes = NUM_HIDDEN;  // this is never used


// Modes (gets set when the light starts up based on stars)
PROGMEM const uint8_t modesNx1[] = { MODESNx1 , 0, HIDDENMODES };
PROGMEM const uint8_t modesNx2[] = { MODESNx2 , 0, HIDDENMODES };
const uint8_t *modesNx;

PROGMEM const uint8_t modes1x1[] = { MODES1x1 , 0, HIDDENMODES };
PROGMEM const uint8_t modes1x2[] = { MODES1x2 , 0, HIDDENMODES };
const uint8_t *modes1x;

PROGMEM const uint8_t modes_pwm1[] = { MODES_PWM1 , 0, HIDDENMODES_PWM };
PROGMEM const uint8_t modes_pwm2[] = { MODES_PWM2 , 0, HIDDENMODES_PWM };
const uint8_t *modes_pwm;

PROGMEM const uint8_t voltage_blinks[] = {
    ADC_0,    // 1 blink  for 0%-25%
    ADC_25,   // 2 blinks for 25%-50%
    ADC_50,   // 3 blinks for 50%-75%
    ADC_75,   // 4 blinks for 75%-100%
    ADC_100,  // 5 blinks for >100%
};

void save_state() {  //central method for writing (with wear leveling)
    uint8_t oldpos=eepos;
    // a single 16-bit write uses less ROM space than two 8-bit writes
    uint16_t eep;

    eepos=(eepos+2)&31;  //wear leveling, use next cell

    eep = mode_idx | (fast_presses << 12) | (modegroup << 8);
    eeprom_write_word((uint16_t *)(eepos), eep);
    eeprom_write_word((uint16_t *)(oldpos), 0xffff);
}
void restore_state() {
    // two 8-bit reads use less ROM space than a single 16-bit write
    uint8_t eep1;
    uint8_t eep2;
    for(eepos=0; (eepos<32); eepos+=2) {
        eep1 = eeprom_read_byte((const uint8_t *)eepos);
        eep2 = eeprom_read_byte((const uint8_t *)eepos+1);
        if (eep1 != 0xff) break;
    }
    if (eepos < 32) {
        mode_idx = eep1;
        fast_presses = (eep2 >> 4);
        modegroup = eep2 & 1;
    }
    else eepos=0;
}

inline void next_mode() {
    mode_idx += 1;
    if (mode_idx >= solid_modes) {
        // Wrap around
        // (note: this also applies when going "forward" from any hidden mode)
        mode_idx = 0;
    }
}

#ifdef OFFTIM3
inline void prev_mode() {
    if (mode_idx > 0) {
        // Regular mode: is between 1 and TOTAL_MODES
        mode_idx -= 1;
    } else {
        // Otherwise, wrap around
        mode_idx = mode_cnt - 1;
    }
    // If we hit the end of the hidden modes, go back to moon
    if (pgm_read_byte(modes_pwm + mode_idx) == 0) {
        mode_idx = 0;
    }
}
#endif

inline void check_stars() {
    // Configure options based on stars
    // 0 being low for soldered, 1 for pulled-up for not soldered
#if 0  // not implemented, STAR2_PIN is used for second PWM channel
    // Moon
    // enable moon mode?
    if ((PINB & (1 << STAR2_PIN)) == 0) {
        modes[mode_cnt++] = MODE_MOON;
    }
#endif
#if 0  // Mode order not as important as mem/no-mem
    // Mode order
    if ((PINB & (1 << STAR3_PIN)) == 0) {
        // High to Low
        mode_dir = -1;
    } else {
        mode_dir = 1;
    }
#endif
    // Memory
    if ((PINB & (1 << STAR3_PIN)) == 0) {
        memory = 1;  // solder to enable memory
    } else {
        memory = 0;  // unsolder to disable memory
    }
}

void count_modes() {
    /*
     * Determine how many solid and hidden modes we have
     * The modes_pwm array should have several values then a zero,
     * then several more values then a zero.  Regular modes are the
     * first group, hidden modes are the second group.
     *
     * (this matters because, in theory, it might have more than one
     *  set of modes to choose from, so we need to count at runtime)
     */
    if (modegroup == 0) {
        solid_modes = NUM_MODES1;
        modesNx = modesNx1;
        modes1x = modes1x1;
        modes_pwm = modes_pwm1;
    } else {
        solid_modes = NUM_MODES2;
        modesNx = modesNx2;
        modes1x = modes1x2;
        modes_pwm = modes_pwm2;
    }
    mode_cnt = solid_modes + 1 + NUM_HIDDEN;
}

#ifdef VOLTAGE_MON
inline void ADC_on() {
    DIDR0 |= (1 << ADC_DIDR);                           // disable digital input on ADC pin to reduce power consumption
    ADMUX  = (1 << REFS0) | (1 << ADLAR) | ADC_CHANNEL; // 1.1v reference, left-adjust, ADC1/PB2
    ADCSRA = (1 << ADEN ) | (1 << ADSC ) | ADC_PRSCL;   // enable, start, prescale
}
#else
inline void ADC_off() {
    ADCSRA &= ~(1<<7); //ADC off
}
#endif

void set_output(uint8_t pwm1, uint8_t pwm2) {
    // Need PHASE to properly turn off the light
    if ((pwm1==0) && (pwm2==0)) {
        TCCR0A = PHASE;
    }
    PWM_LVL = pwm1;
    ALT_PWM_LVL = pwm2;
}

void set_mode(mode) {
    TCCR0A = pgm_read_byte(modes_pwm + mode);
    set_output(pgm_read_byte(modesNx + mode), pgm_read_byte(modes1x + mode));
    /*
    // Only set output for solid modes
    uint8_t out = pgm_read_byte(modesNx + mode);
    if ((out < 250) || (out == 255)) {
        set_output(pgm_read_byte(modesNx + mode), pgm_read_byte(modes1x + mode));
    }
    */
}

#ifdef VOLTAGE_MON
uint8_t get_voltage() {
    // Start conversion
    ADCSRA |= (1 << ADSC);
    // Wait for completion
    while (ADCSRA & (1 << ADSC));
    // See if voltage is lower than what we were looking for
    return ADCH;
}
#endif

void blink(uint8_t val)
{
    for (; val>0; val--)
    {
        set_output(0,20);
        _delay_ms(100);
        set_output(0,0);
        _delay_ms(400);
    }
}

int main(void)
{
    uint8_t cap_val;

    // Read the off-time cap *first* to get the most accurate reading
    // Start up ADC for capacitor pin
    DIDR0 |= (1 << CAP_DIDR);                           // disable digital input on ADC pin to reduce power consumption
    ADMUX  = (1 << REFS0) | (1 << ADLAR) | CAP_CHANNEL; // 1.1v reference, left-adjust, ADC3/PB3
    ADCSRA = (1 << ADEN ) | (1 << ADSC ) | ADC_PRSCL;   // enable, start, prescale

    // Wait for completion
    while (ADCSRA & (1 << ADSC));
    // Start again as datasheet says first result is unreliable
    ADCSRA |= (1 << ADSC);
    // Wait for completion
    while (ADCSRA & (1 << ADSC));
    cap_val = ADCH; // save this for later

    // All ports default to input, but turn pull-up resistors on for the stars (not the ADC input!  Made that mistake already)
    // only one star, because one is used for PWM channel 2
    // and the other is used for the off-time capacitor
    PORTB = (1 << STAR3_PIN);

    // Set PWM pin to output
    DDRB |= (1 << PWM_PIN);     // enable main channel
    DDRB |= (1 << ALT_PWM_PIN); // enable second channel

    // Set timer to do PWM for correct output pin and set prescaler timing
    //TCCR0A = 0x23; // phase corrected PWM is 0x21 for PB1, fast-PWM is 0x23
    //TCCR0B = 0x01; // pre-scaler for timer (1 => 1, 2 => 8, 3 => 64...)
    TCCR0A = PHASE;
    // Set timer to do PWM for correct output pin and set prescaler timing
    TCCR0B = 0x01; // pre-scaler for timer (1 => 1, 2 => 8, 3 => 64...)

    // Read config values and saved state
    check_stars();
    restore_state();
    // Enable the current mode group
    count_modes();


    if (cap_val > CAP_SHORT) {
        // Indicates they did a short press, go to the next mode
        next_mode(); // Will handle wrap arounds
        if (fast_presses < 15) fast_presses ++;
#ifdef OFFTIM3
    } else if (cap_val > CAP_MED) {
        // User did a medium press, go back one mode
        prev_mode(); // Will handle "negative" modes and wrap-arounds
        fast_presses = 0;
#endif
    } else {
        // Long press, keep the same mode
        // ... or reset to the first mode
        fast_presses = 0;
        if (! memory) {
            // Reset to the first mode
            mode_idx = 0;
        }
    }
    save_state();

    // Turn off ADC
    //ADC_off();

    // Charge up the capacitor by setting CAP_PIN to output
    DDRB  |= (1 << CAP_PIN);    // Output
    PORTB |= (1 << CAP_PIN);    // High

    // Turn features on or off as needed
    #ifdef VOLTAGE_MON
    ADC_on();
    #else
    ADC_off();
    #endif
    //ACSR   |=  (1<<7); //AC off

    // Enable sleep mode set to Idle that will be triggered by the sleep_mode() command.
    // Will allow us to go idle between WDT interrupts
    //set_sleep_mode(SLEEP_MODE_IDLE);  // not used due to blinky modes

    uint8_t output;
#ifdef NON_WDT_TURBO
    uint8_t ticks = 0;
#endif
#ifdef VOLTAGE_MON
    uint8_t lowbatt_cnt = 0;
    uint8_t i = 0;
    uint8_t voltage;
    // Make sure voltage reading is running for later
    ADCSRA |= (1 << ADSC);
#endif
    while(1) {
        output = pgm_read_byte(modesNx + mode_idx);
        if (fast_presses == 0x0f) {  // Config mode
            _delay_ms(1000);  // wait for user to stop fast-pressing button
            fast_presses = 0; // exit this mode after one use
            mode_idx = 0;

            // Toggle the mode group, blink, then exit
            modegroup ^= 1;
            save_state();
            blink(1);
        }
        else if (output == STROBE) {
            set_output(255,255);
            _delay_ms(50);
            set_output(0,0);
            _delay_ms(50);
        }
        else if (output == BATTCHECK) {
            uint8_t blinks = 0;
            // turn off and wait one second before showing the value
            // (also, ensure voltage is measured while not under load)
            set_output(0,0);
            _delay_ms(1000);
            voltage = get_voltage();
            voltage = get_voltage(); // the first one is unreliable
            // division takes too much flash space
            //voltage = (voltage-ADC_LOW) / (((ADC_42 - 15) - ADC_LOW) >> 2);
            // a table uses less space than 5 logic clauses
            for (i=0; i<sizeof(voltage_blinks); i++) {
                if (voltage > pgm_read_byte(voltage_blinks + i)) {
                    blinks ++;
                }
            }

            // blink up to five times to show voltage
            // (~0%, ~25%, ~50%, ~75%, ~100%, >100%)
            blink(blinks);
            _delay_ms(1000);  // wait at least 1 second between readouts
        }
        else {  // Regular non-hidden solid mode
            set_mode(mode_idx);
            // This part of the code will mostly replace the WDT tick code.
#ifdef NON_WDT_TURBO
            // Do some magic here to handle turbo step-down
            if (ticks < 255) ticks++;
            if ((ticks > TURBO_TIMEOUT) 
                    && (output == TURBO)) {
                mode_idx = solid_modes - 2; // step down to second-highest mode
                set_mode(mode_idx);
                save_state();
            }
#endif
            // Otherwise, just sleep.
            _delay_ms(500);

            // If we got this far, the user has stopped fast-pressing.
            // So, don't enter config mode.
            fast_presses = 0;
            save_state();
        }
#ifdef VOLTAGE_MON
#if 1
        if (ADCSRA & (1 << ADIF)) {  // if a voltage reading is ready
            voltage = ADCH; // get_voltage();
            // See if voltage is lower than what we were looking for
            //if (voltage < ((mode_idx <= 1) ? ADC_CRIT : ADC_LOW)) {
            if (voltage < ADC_LOW) {
                lowbatt_cnt ++;
            } else {
                lowbatt_cnt = 0;
            }
            // See if it's been low for a while, and maybe step down
            if (lowbatt_cnt >= 8) {
                // DEBUG: blink on step-down:
                //set_output(0,0);  _delay_ms(100);
                i = mode_idx; // save space by not accessing mode_idx more than necessary
                // properly track hidden vs normal modes
                if (i >= solid_modes) {
                    // step down from blinky modes to medium
                    i = 3;
                } else if (i > 0) {
                    // step down from solid modes one at a time
                    i -= 1;
                } else { // Already at the lowest mode
                    i = 0;
                    // Turn off the light
                    set_output(0,0);
                    // Power down as many components as possible
                    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
                    sleep_mode();
                }
                set_mode(i);
                mode_idx = i;
                save_state();
                lowbatt_cnt = 0;
                // Wait at least 2 seconds before lowering the level again
                _delay_ms(250);  // this will interrupt blinky modes
            }

            // Make sure conversion is running for next time through
            ADCSRA |= (1 << ADSC);
        }
#endif
#endif
        //sleep_mode();  // incompatible with blinky modes
    }

    //return 0; // Standard Return Code
}
