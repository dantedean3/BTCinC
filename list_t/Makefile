CC=gcc
CFLAGS=-Wall -Wextra -g
OBJS= list_t.o tester.o

all: tester

tester: $(OBJS)
	$(CC) $(CFLAGS) -o tester $(OBJS)

list_t.o: list_t.c list_t.h
	$(CC) $(CFLAGS) -c list_t.c

tester.o: tester.c list_t.h
	$(CC) $(CFLAGS) -c tester.c

clean:
	rm -f *.o tester
