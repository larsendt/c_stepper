#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "gpio.h"

#define VALUE_FILE "/sys/class/gpio/gpio%d/value"
#define MODE_FILE "/sys/class/gpio/gpio%d/direction"
#define EXPORT_FILE "/sys/class/gpio/export"

int gpio_init(gpio *g, int pin, pin_mode mode) {
    char mode_path[40];
    sprintf(mode_path, MODE_FILE, pin);
    char value_path[40];
    sprintf(value_path, VALUE_FILE, pin);

    g->pin = pin;
    g->f = NULL;
    g->cur_mode = mode;
    g->cur_value = LOW;
    g->open = 0;

    FILE *exf = fopen(EXPORT_FILE, "w");
    if(!exf) {
#ifdef GPIO_WARNINGS
        fprintf(stderr, "Failed to open export file: %s (%s)\n", EXPORT_FILE, strerror(errno));
#endif
        return 0;
    }

    // Make sure the pin we want is enabled
    fprintf(exf, "%d", pin);
    fflush(exf);

    FILE *modef = fopen(mode_path, "w");
    if(!modef) {
#ifdef GPIO_WARNINGS
        fprintf(stderr, "Failed to open mode file: %s (%s)\n", mode_path, strerror(errno));
#endif
        return 0;
    }

    if(mode == READ) {
        fprintf(modef, "in");
    }
    else if(mode == WRITE) {
        fprintf(modef, "out");
    }
    else {
#ifdef GPIO_WARNINGS
        fprintf(stderr, "Bad GPIO mode: %d\n", mode);
#endif
        return 0;
    }

    fclose(modef);

    if(mode == READ) {
        g->f = fopen(value_path, "r");
    }
    else if(mode == WRITE) {
        g->f = fopen(value_path, "w");
    }

    if(!g->f) {
#ifdef GPIO_WARNINGS
        fprintf(stderr, "Failed to open value file: %s (%s)\n", value_path, strerror(errno));
#endif
        return 0;
    } 

    g->open = 1;
    return 1;
}

void gpio_term(gpio *g) {
    fclose(g->f);
    g->open = 0;
}

int gpio_set_value(gpio *g, pin_value value) {
    if(value == HIGH) {
        fprintf(g->f, "1");
    }
    else if(value == LOW) {
        fprintf(g->f, "0");
    }
    else {
#ifdef GPIO_WARNINGS
        fprintf(stderr, "Bad GPIO value: %d\n", value);
#endif
        return 0;
    }

    fflush(g->f);
    return 1;
}

