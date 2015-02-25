#ifndef _THREADED_STEPPER_H_
#define _THREADED_STEPPER_H_

#include "stepper.h"

typedef struct threaded_stepper {
    stepper_control *control;
    pthread_t thread;
    int steps;
    int usdelay;
    int direction;
    stepper_microstep ustep;
} threaded_stepper;

int tstepper_init(threaded_stepper *s, stepper_control *c);

// calling tstepper_rot while the stepper is in motion will immediately 
// interrupt the stepper
void tstepper_rot(threaded_stepper *s, int deg, int dir, int rpm, stepper_microstep ustep);

// returns the number of remining steps for the current plan
int tstepper_remaining_steps(threaded_stepper *s);

void tstepper_term(threaded_stepper *s);


#endif
