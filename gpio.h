#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    READ,
    WRITE
} pin_mode;

typedef enum {
    HIGH,
    LOW,
} pin_value;

typedef struct gpio {
    FILE *f;
    pin_mode cur_mode;
    pin_value cur_value;
    int pin;
    int open;
} gpio;


int gpio_init(gpio *g, int pin, pin_mode mode);
void gpio_term(gpio *g);
int gpio_set_value(gpio *g, pin_value value);

#endif

