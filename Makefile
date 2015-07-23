# Makefile for Red Bull Moth Project 2015
ODIR=src/obj
CC=gcc
CFLAGS=-Wall -pthread -lmraa
FILE=moth

_OBJ=main.o gps.o stringpot.o log.o i2c.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

_HEADERS=prototypes.h log.h defines.h
HEADERS=$(patsubst %,src/%,$(_HEADERS))

$(ODIR)/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(FILE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f moth $(ODIR)/*.o $(SDIR)/*~
