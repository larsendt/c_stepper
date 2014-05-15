CFLAGS = -Wall -g -std=gnu99 -pthread

all: stepper_control 

stepper_control: main.c libstepper.so
	gcc $(CFLAGS) main.c -fPIC -o stepper_control -lrt -lstepper

libstepper.so: gpio.o stepper.o
	gcc -shared -fPIC gpio.o stepper.o -o libstepper.so -lrt

stepper.o: stepper.c stepper.h
	gcc $(CFLAGS) -fPIC -c stepper.c -o stepper.o -lrt

gpio.o: gpio.c gpio.h
	gcc $(CFLAGS) -fPIC -c gpio.c -o gpio.o -lrt

clean:
	rm -f *.o stepper_control libstepper.so
