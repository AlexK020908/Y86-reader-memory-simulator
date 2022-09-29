CC = g++
CFLAGS = -Wall -g

main: main.o sim.o
	$(CC) $(CFLAGS) -o main main.o sim.o

main.o: main.cpp 
	$(CC) $(CFLAGS) -c main.cpp

sim.o: sim.h