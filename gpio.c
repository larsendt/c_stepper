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

    FILE *exf = fopen(EXPORT_FILE, "w");
    if(!exf) {
        fprintf(stderr, "Failed to open export file: %s\n", EXPORT_FILE);
        fprintf(stderr, "%s\n", strerror(errno));
        return 0;
    }

    // Make sure the pin we want is enabled
    fprintf(exf, "%d", pin);
    fflush(exf);

    FILE *modef = fopen(mode_path, "w");
    if(!modef) {
        fprintf(stderr, "Failed to open mode file: %s\n", mode_path);
        fprintf(stderr, "%s\n", strerror(errno));
        return 0;
    }

    if(mode == READ) {
        fprintf(modef, "in");
    }
    else if(mode == WRITE) {
        fprintf(modef, "out");
    }
    else {
        fprintf(stderr, "Bad GPIO mode: %d\n", mode);
        return 0;
    }

    fclose(modef);

    g->pin = pin;
    if(mode == READ) {
        g->f = fopen(value_path, "r");
    }
    else if(mode == WRITE) {
        g->f = fopen(value_path, "w");
    }

    if(!g->f) {
        fprintf(stderr, "Failed to open value file: %s\n", value_path);
        fprintf(stderr, "%s\n", strerror(errno));
        return 0;
    } 

    g->cur_mode = mode;
    g->cur_value = LOW;
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
        fprintf(stderr, "Bad GPIO value: %d\n", value);
        return 0;
    }

    fflush(g->f);
    return 1;
}

