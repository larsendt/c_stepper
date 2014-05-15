#include "stepper.h"
#include "gpio.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sched.h>

#define STEPS_PER_REV 400
#define ftime(t) ((double)t.tv_sec + (t.tv_nsec / 1e9))

void *_stepper_main_function(void *params);

void start_stepper_thread(stepper_control *s) {
    int ok;
    s->dir = 0;
    s->enabled = 0;
    ok = pthread_create(&(s->thread_handle), NULL, &_stepper_main_function, s);
    if(ok != 0) {
        fprintf(stderr, "Error on stepper thread start!\n");
        fprintf(stderr, "%s\n", strerror(errno));
        s->has_error = 1;
    }
    s->has_error = 0;
}


void stop_stepper_thread(stepper_control *s) {
    s->enabled = 0;
    pthread_cancel(s->thread_handle);
}

void *_stepper_main_function(void *params) {
    stepper_control *s = params;
    gpio g;
    gpio_init(&g, 30, WRITE);

    int on = 0;
    struct timespec sleepval;
    sleepval.tv_sec = 0;
    sleepval.tv_nsec = 100000;

    while(1) {
        if(s->enabled) {
            if(on) {
                gpio_set_value(&g, HIGH);
                on = 0;
            }
            else {
                gpio_set_value(&g, LOW);
                on = 1;
            }
        }
        else {
            gpio_set_value(&g, LOW);
        }
        nanosleep(&sleepval, NULL);
    }
}
