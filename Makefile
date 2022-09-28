PROGRAMS = sim
INCLUDES = -I.

CFLAGS = -g -Wall $(INCLUDES)

all: $(PROGRAMS)

%.o:%.c'
	$(CC) $(CFLAGS) -c $<

TESTER_OBJS = sim.o main.o

sim: $(TESTER_OBJS)
	$(CC) -o sim $(CFLAGS) $(TESTER_OBJS) $(KEEP_OBJS)

clean:
	rm -rf $(PROGRAMS) $(TESTER_OBJS) core
