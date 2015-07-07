# Makefile for Red Bull Moth Project 2015
IDIR=../include
ODIR=obj
CC=gcc
CFLAGS=-Wall -pthread -lmraa

_DEPS=mraa.h mraa/aio.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=main.o gps.o stringpot.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

moth: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 
	
.PHONY: clean

clean:
	rm -f #(ODIR)/*.o *~ moth $(IDIR)/*~ 
