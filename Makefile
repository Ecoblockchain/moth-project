# Makefile for Red Bull Moth Project 2015
ODIR=src/obj
CC=gcc
CFLAGS=-Wall -pthread -lmraa
FILE=moth

_OBJ=main.o gps.o stringpot.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(FILE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 
	
.PHONY: clean

clean:
	rm -f moth $(ODIR)/*.o $(SDIR)/*~
