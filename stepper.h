#ifndef _STEPPER_H_
#define _STEPPER_H_

#include "gpio.h"

typedef enum stepper_microstep {
    FULL_STEP = 0,
    HALF_STEP = 1,
    QUARTER_STEP = 2,
    EIGHTH_STEP = 3,
    SIXTEENTH_STEP = 7,
} stepper_microstep;


typedef struct stepper_control {
    gpio step_g;
    gpio dir_g;
    gpio m1_g;
    gpio m2_g;
    gpio m3_g;
} stepper_control;

int stepper_init(stepper_control *s, int step_pin, int dir_pin, int m1_pin, int m2_pin, int m3_pin);
void stepper_destroy(stepper_control *s);
void stepper_rot(stepper_control *s, int deg, int dir, int rpm, stepper_microstep ustep);
void stepper_step(stepper_control *s, int steps, int dir, int usdelay, stepper_microstep ustep);
int stepper_get_usdelay(int rpm, stepper_microstep ustep);
int stepper_get_steps(int deg, stepper_microstep ustep);

#endif
