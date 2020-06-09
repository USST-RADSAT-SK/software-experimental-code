CC      = gcc
CFLAGS  = -Wall -Wextra
RM      = rm -f
LDFLAGS = -lzmq

default: all

all: sat groundStation

sat: sat.o
	$(CC) -o sat sat.o $(LDFLAGS)

sat.o: sat.c message.h
	$(CC) $(CFLAGS) -c sat.c

groundStation: groundStation.o
	$(CC) -o groundStation groundStation.o $(LDFLAGS)

groundStation.o: groundStation.c message.h
	$(CC) $(CFLAGS) -c groundStation.c

.PHONY: clean

clean:
	rm *.o sat groundStation
