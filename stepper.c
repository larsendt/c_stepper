#include "stepper.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define STEPS_PER_DEG (400.0 / 360.0)
#define deg_to_step(deg) ((int)(STEPS_PER_DEG * deg))
#define rpm_to_usdelay(rpm, rev_steps) ((int)((60000000.0 / rpm) / (double)rev_steps))

int stepper_init(stepper_control *s, int step_pin, int dir_pin, int m1_pin, int m2_pin, int m3_pin) {
    int ok;
    ok = gpio_init(&(s->step_g), step_pin, WRITE);
    if(!ok) {
        fprintf(stderr, "Failed to init step pin (GPIO %d)\n", step_pin);
        return 0;
    }
 
    ok = gpio_init(&(s->dir_g), dir_pin, WRITE);
    if(!ok) {
        fprintf(stderr, "Failed to init dir pin (GPIO %d)\n", dir_pin);
        return 0;
    }

    ok = gpio_init(&(s->m1_g), m1_pin, WRITE);
    if(!ok) {
        fprintf(stderr, "Failed to init m1 pin (GPIO %d)\n", m1_pin);
        return 0;
    }

    ok = gpio_init(&(s->m2_g), m2_pin, WRITE);
    if(!ok) {
        fprintf(stderr, "Failed to init m2 pin (GPIO %d)\n", m2_pin);
        return 0;
    }

    ok = gpio_init(&(s->m3_g), m3_pin, WRITE);
    if(!ok) {
        fprintf(stderr, "Failed to init m3 pin (GPIO %d)\n", m3_pin);
        return 0;
    }

    return 1;
}


void stepper_destroy(stepper_control *s) {
    gpio_term(&(s->step_g));
    gpio_term(&(s->dir_g));
}


void step_sleep(int us) {
    struct timespec sleepval;
    sleepval.tv_sec = 0;
    sleepval.tv_nsec = us * 1000;
    nanosleep(&sleepval, NULL);
}


void stepper_rot(stepper_control *s, int deg, int dir, int rpm, stepper_microstep ustep) {
    int needed_steps = deg_to_step(deg);
    int rev_steps = 400;
    if(ustep == US_HALF) { 
        needed_steps *= 2;
        rev_steps *= 2;  
    } 
    else if(ustep == US_QUARTER) { 
        needed_steps *= 4;
        rev_steps *= 4;
    }
    else if(ustep == US_EIGHTH) {
         needed_steps *= 8;
         rev_steps *= 8;
    }
    else if(ustep == US_SIXTEENTH) { 
        needed_steps *= 16;
        rev_steps *= 16;
    }

    int needed_delay_us = rpm_to_usdelay(rpm, rev_steps) / 2;

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
    for(int i = 0; i < needed_steps*2; i++) {
        if(on) {
            gpio_set_value(&(s->step_g), HIGH);
            on = 0;
        }
        else {
            gpio_set_value(&(s->step_g), LOW);
            on = 1;
        }
        step_sleep(needed_delay_us);
    }     
}

