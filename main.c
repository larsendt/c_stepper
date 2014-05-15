#include <stdio.h>
#include <unistd.h>
#include "stepper.h"

int main(void) {
    stepper_control s;
    stepper_init(&s, 31, 30, 48, 51, 15);
    
    int dir = 0;
    for(int i = 0; i < 4; i++) {
        stepper_rot(&s, 180, dir, 10, US_SIXTEENTH);
        dir = !dir;
    }

    stepper_destroy(&s);
    return 0;
}
