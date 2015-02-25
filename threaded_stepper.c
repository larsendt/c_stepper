#include "threaded_stepper.h"
#include "stepper.h"
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>


void *tstep_thread_fn(void *params);

int tstepper_init(threaded_stepper *s, stepper_control *c) {
    s->control = c;
    s->steps = 0;
    s->usdelay = 0;
    s->direction = 0;
    s->ustep = FULL_STEP;
    pthread_create(&(s->thread), NULL, tstep_thread_fn, s);
    return 0;
}

void tstepper_term(threaded_stepper *s) {
    pthread_cancel(s->thread);
}

void tstepper_rot(threaded_stepper *s, int deg, int dir, int rpm, stepper_microstep ustep) {
    int steps = stepper_get_usdelay(rpm, ustep);
    int usdelay = stepper_get_steps(deg, ustep);
    s->steps = steps;
    s->usdelay = usdelay;
    s->direction = dir;
    s->ustep = ustep;
}

int tstepper_remaining_steps(threaded_stepper *s) {
    return s->steps;
}

void *tstep_thread_fn(void *params) {
    threaded_stepper *s = params;
    while(1) {
        if(s->steps > 0) {
            int steps_to_take = 10;
            if(s->steps < steps_to_take) {
                steps_to_take = s->steps;
            }
            
            //stepper_step(s->control, steps_to_take, s->dir, s->usdelay, s->ustep);
            usleep(10000);
            s->steps -= steps_to_take;
        }
        else {
            usleep(10000);
        }
    }
    return NULL;
}
