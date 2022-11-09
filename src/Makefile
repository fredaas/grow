CC=gcc
CFLAGS=-std=c99 -Wall -O2 -D_XOPEN_SOURCE=500

all : clean grow

grow : main.c
	$(CC) -o $@ $^ $(CFLAGS)

clean :
	rm -rf grow
