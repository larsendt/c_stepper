CFLAGS = -Wall -g -std=gnu99 -pthread -DSTEPPER_WARNINGS -DGPIO_WARNINGS
CC=clang
LIBS = -lm

all: stepper_control 

stepper_control: main.c libstepper.so
	$(CC) $(CFLAGS) main.c -fPIC -o stepper_control $(LIBS) -lstepper

libstepper.so: gpio.o stepper.o threaded_stepper.o
	$(CC) -shared gpio.o stepper.o threaded_stepper.o -o libstepper.so $(LIBS)

threaded_stepper.o: threaded_stepper.c threaded_stepper.h
	$(CC) $(CFLAGS) -fPIC -c threaded_stepper.c -o threaded_stepper.o

stepper.o: stepper.c stepper.h
	$(CC) $(CFLAGS) -fPIC -c stepper.c -o stepper.o

gpio.o: gpio.c gpio.h
	$(CC) $(CFLAGS) -fPIC -c gpio.c -o gpio.o

clean:
	rm -f *.o stepper_control libstepper.so
