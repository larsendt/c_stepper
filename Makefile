CFLAGS = -Wall -g -std=gnu99 -pthread
LIBS = -lm

all: stepper_control 

stepper_control: main.c libstepper.so
	gcc $(CFLAGS) main.c -fPIC -o stepper_control $(LIBS) -lstepper

libstepper.so: gpio.o stepper.o
	gcc -shared -fPIC gpio.o stepper.o -o libstepper.so $(LIBS)

stepper.o: stepper.c stepper.h
	gcc $(CFLAGS) -fPIC -c stepper.c -o stepper.o

gpio.o: gpio.c gpio.h
	gcc $(CFLAGS) -fPIC -c gpio.c -o gpio.o

clean:
	rm -f *.o stepper_control libstepper.so
