# Makefile for Red Bull Moth Project 2015
# Author: James Vaughan
SDIR=src
ODIR=$(SDIR)/obj
CC=gcc
RM=rm -rf
CFLAGS=-Wall
LDLIBS=-pthread -lm -lrt
FILE=moth

_OBJ=main.o gps.o log.o sonar.o switch.o imu.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

_HEADERS=sonar.h log.h gps.h switch.h imu.h
HEADERS=$(patsubst %,src/%,$(_HEADERS))

$(ODIR)/%.o: $(SDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(FILE): $(OBJ)
	$(CC) $(LDLIBS) -o $@ $^

.PHONY: clean

clean:
	$(RM) $(FILE) $(ODIR)/* $(SDIR)/*~

clean-logs:
	$(RM) /root/log/*

install:
	cp $(FILE) /bin/moth

push:
	rsync -rP --delete . root@192.168.7.2:~/moth-project
