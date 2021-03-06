#include "stepper.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define STEPS_PER_DEG (400.0 / 360.0)
#define deg_to_step(deg) ((int)(STEPS_PER_DEG * deg))
#define rpm_to_usdelay(rpm, rev_steps) ((int)((60000000.0 / rpm) / (double)rev_steps))
#define tdiff(start, stop) ((stop.tv_sec - start.tv_sec) + ((stop.tv_nsec - start.tv_nsec) / 1e9))

int stepper_init(stepper_control *s, int step_pin, int dir_pin, int m1_pin, int m2_pin, int m3_pin) {
    int ok;
    ok = gpio_init(&(s->step_g), step_pin, WRITE);
    if(!ok) {
#ifdef STEPPER_WARNINGS
        fprintf(stderr, "Failed to init step pin (GPIO %d)\n", step_pin);
#endif
        return 0;
    }
 
    ok = gpio_init(&(s->dir_g), dir_pin, WRITE);
    if(!ok) {
#ifdef STEPPER_WARNINGS
        fprintf(stderr, "Failed to init dir pin (GPIO %d)\n", dir_pin);
#endif
        return 0;
    }

    ok = gpio_init(&(s->m1_g), m1_pin, WRITE);
    if(!ok) {
#ifdef STEPPER_WARNINGS
        fprintf(stderr, "Failed to init m1 pin (GPIO %d)\n", m1_pin);
#endif
        return 0;
    }

    ok = gpio_init(&(s->m2_g), m2_pin, WRITE);
    if(!ok) {
#ifdef STEPPER_WARNINGS
        fprintf(stderr, "Failed to init m2 pin (GPIO %d)\n", m2_pin);
#endif 
        return 0;
    }

    ok = gpio_init(&(s->m3_g), m3_pin, WRITE);
    if(!ok) {
#ifdef STEPPER_WARNINGS
        fprintf(stderr, "Failed to init m3 pin (GPIO %d)\n", m3_pin);
#endif
        return 0;
    }

    return 1;
}


void stepper_destroy(stepper_control *s) {
    gpio_term(&(s->step_g));
    gpio_term(&(s->dir_g));
    gpio_term(&(s->m1_g));
    gpio_term(&(s->m2_g));
    gpio_term(&(s->m3_g));
}


void step_sleep(int us) {
    struct timespec loopstart;
    struct timespec loopstop;

    // For values around 1us, the inner while loop becomes dominated by
    // the clock_gettime call, so instead we go in increments of 10us. Gives
    // much better accuracy (down to 10us, of course), but that's good enough
    // for stepper motors.
    //
    // Error stats (avg of 100-100000 iterations, depending on how long I feel 
    //              like measuring for):
    // 100ms: +2.45% of expected
    // 10ms: +1.95%
    // 1ms: +3.70%
    // 100us: +8.89%
    // 10us: +6.39%
    // 1us: +205% <- things kinda fall apart here
    for(int i = 0; i < (us/10); i++) {
        clock_gettime(CLOCK_REALTIME, &loopstart);
        clock_gettime(CLOCK_REALTIME, &loopstop);
        while(tdiff(loopstart, loopstop) < 0.00001) {
            clock_gettime(CLOCK_REALTIME, &loopstop);
        }
    }
}

void measure_sleep() {
    struct timespec start;
    struct timespec stop;

    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 1000; i++) {
        step_sleep(1000);
    }
    clock_gettime(CLOCK_REALTIME, &stop);
    printf("Sleep measurement: %.8f\n", tdiff(start, stop));
}

int stepper_get_usdelay(int rpm, stepper_microstep ustep) {
    int rev_steps = 400;
    if(ustep == HALF_STEP) { 
        rev_steps *= 2;  
    } 
    else if(ustep == QUARTER_STEP) { 
        rev_steps *= 4;
    }
    else if(ustep == EIGHTH_STEP) {
         rev_steps *= 8;
    }
    else if(ustep == SIXTEENTH_STEP) { 
        rev_steps *= 16;
    }

    return rpm_to_usdelay(rpm, rev_steps) / 2;
}

int stepper_get_steps(int deg, stepper_microstep ustep) {
    int needed_steps = deg_to_step(deg);
    if(ustep == HALF_STEP) { 
        needed_steps *= 2;
    } 
    else if(ustep == QUARTER_STEP) { 
        needed_steps *= 4;
    }
    else if(ustep == EIGHTH_STEP) {
         needed_steps *= 8;
    }
    else if(ustep == SIXTEENTH_STEP) { 
        needed_steps *= 16;
    }
    return needed_steps;
}

void stepper_step(stepper_control *s, int steps, int dir, int usdelay, stepper_microstep ustep) {
    if(dir == 0) {
        gpio_set_value(&(s->dir_g), LOW);
    }
    else {
        gpio_set_value(&(s->dir_g), HIGH);
    }

    pin_value m1, m2, m3;
    m1 = ustep & 1 ? HIGH : LOW;
    m2 = (ustep >> 1) & 1 ? HIGH : LOW;
    m3 = (ustep >> 2) & 1 ? HIGH : LOW;

    gpio_set_value(&(s->m1_g), m1);
    gpio_set_value(&(s->m2_g), m2);
    gpio_set_value(&(s->m3_g), m3);

    int on = 0;
    for(int i = 0; i < steps*2; i++) {
        if(on) {
            gpio_set_value(&(s->step_g), HIGH);
            on = 0;
        }
        else {
            gpio_set_value(&(s->step_g), LOW);
            on = 1;
        }
        step_sleep(usdelay);
    }     
}

void stepper_rot(stepper_control *s, int deg, int dir, int rpm, stepper_microstep ustep) {
    int needed_delay_us = stepper_get_usdelay(rpm, ustep);
    int needed_steps = stepper_get_steps(deg, ustep);
    stepper_step(s, needed_steps, dir, needed_delay_us, ustep);
}

