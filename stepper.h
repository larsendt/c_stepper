#ifndef _STEPPER_H_
#define _STEPPER_H_

#include <pthread.h>

typedef struct stepper_control {
    int dir;
    int enabled;
    int has_error;
    pthread_t thread_handle;
} stepper_control;

void start_stepper_thread(stepper_control *s);
void stop_stepper_thread(stepper_control *s);

#endif
