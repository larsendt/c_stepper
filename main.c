#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "stepper.h"

int main(void) {
    stepper_control s;
    stepper_init(&s, 31, 30, 48, 51, 15);
    stepper_rot(&s, 3600, 0, 60, US_FULL);
    stepper_destroy(&s);
    return 0;
}
