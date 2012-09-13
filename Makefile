OUT 	= status

#variables
CC = gcc
CFLAGS = -Wall -g -Wextra

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(OUT)

tags: $(SRC)
	ctags *.c *.h

.c.o:
	$(CC) -c $(CFLAGS) $<

$(OUT): $(OBJ)
	$(CC) -o $@ $(OBJ)

clean:
	rm $(OBJ) $(OUT)
