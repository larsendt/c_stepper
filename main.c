#include <stdio.h>
#include <unistd.h>
#include "stepper.h"

int main(void) {
    stepper_control s;
    start_stepper_thread(&s);
    s.enabled = 1;

    for(int i = 0; i < 5; i++) {
        printf("sleep %d\n", i);
        sleep(1);
    }

    stop_stepper_thread(&s);
    return 0;
}
