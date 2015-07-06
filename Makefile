# Makefile for Red Bull Moth Project 2015

all: main.c gps.c
	gcc -Wall -pthread -o moth main.c gps.c

clean:
	$(RM) moth
