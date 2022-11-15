CC=gcc
CFLAGS=-std=c99 -Wall -O2 -D_XOPEN_SOURCE=500 -static
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

all : clean grow

grow : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJ) : %.o : %.c
	$(CC) -c -o $@ $<

clean :
	rm -rf grow *.o
