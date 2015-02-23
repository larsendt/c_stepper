#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "stepper.h"

int main(void) {
    stepper_control s;
    stepper_init(&s, 31, 30, 48, 51, 15);
    
    int dir = 0;
    int rpm = 0;
    for(int ri = 0; ri < 4; ri++) {
        rpm += 15;
        printf("Iteration %d - rpm: %d\n", ri, rpm);

        printf("Full step\n");
        for(int i = 0; i < 2; i++) {
            stepper_rot(&s, 180, dir, rpm, US_FULL);
            dir = !dir;
        }

        printf("Half step\n");
        for(int i = 0; i < 2; i++) {
            stepper_rot(&s, 180, dir, rpm, US_HALF);
            dir = !dir;
        }

        printf("Quarter step\n");
        for(int i = 0; i < 2; i++) {
            stepper_rot(&s, 180, dir, rpm, US_QUARTER);
            dir = !dir;
        }

        printf("Eighth step\n");
        for(int i = 0; i < 2; i++) {
            stepper_rot(&s, 180, dir, rpm, US_EIGHTH);
            dir = !dir;
        }

        printf("Sixteenth step\n");
        for(int i = 0; i < 2; i++) {
            stepper_rot(&s, 180, dir, rpm, US_SIXTEENTH);
            dir = !dir;
        }
    }

    stepper_destroy(&s);
    return 0;
}
