# Makefile for Red Bull Moth Project 2015
IDIR=include
SDIR=src
ODIR=$(SDIR)/obj
CC=gcc
CFLAGS=-Wall -pthread -lmraa

_DEPS=mraa.h mraa/aio.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=main.o gps.o stringpot.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

moth: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 
	
.PHONY: clean

clean:
	rm -f moth $(ODIR)/*.o $(SDIR)/*~ $(IDIR)/*~ 
