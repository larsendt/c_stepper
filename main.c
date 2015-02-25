#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "stepper.h"
#include "threaded_stepper.h"

int main(void) {
    stepper_control s;
    int ok = stepper_init(&s, 31, 30, 48, 51, 15);
    if(!ok) {
        fprintf(stderr, "Failed to initialize stepper\n");
        return 1;
    }

    srand(time(NULL));

    measure_sleep();

    threaded_stepper t;
    tstepper_init(&t, &s);

    while(1) {
        if(tstepper_remaining_steps(&t) <= 0) {
            //int deg = rand() % 360;
            int deg = 90;
            int dir = rand() % 2;
            int rpm = (rand() % 60) + 1;
            printf("main: new rotation - deg=%d dir=%d rpm=%d\n", deg, dir, rpm);
            tstepper_rot(&t, deg, dir, rpm, QUARTER_STEP);
        }
    }

    tstepper_term(&t);
    //stepper_destroy(&s);
    return 0;
}
