CC      = gcc
CFLAGS  = -Wall
.PHONY: all
all: oss worker
oss: oss.o
	$(CC) -o $@ $^
worker: worker.o
	$(CC) -o $@ $^
%.o: %.c
	$(CC) -c $(CFLAGS) $*.c

clean: 
	rm *.o oss worker
