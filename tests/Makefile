# Compiler
CC=gcc
# Compiler flags
CFLAGS=-Wall -Wextra
# Dynamic library flags
LFLAGS=-lGPR
# Concatenated list of source files
SRCS=$(wildcard *.c)
# Concatenated list of executable files
ELFS=$(SRCS:.c=)

all : $(ELFS)

% : %.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

clean:
	find . -type f -executable -delete
