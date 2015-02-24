#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "stepper.h"

int main(void) {
    stepper_control s;
    int ok = stepper_init(&s, 31, 30, 48, 51, 15);

    if(!ok) {
        fprintf(stderr, "Failed to initialize stepper\n");
        return 1;
    }

    stepper_rot(&s, 3600, 0, 60, US_FULL);
    stepper_destroy(&s);
    return 0;
}
