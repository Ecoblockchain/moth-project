# Makefile for Red Bull Moth Project 2015
SDIR=src
ODIR=$(SDIR)/obj
CC=gcc
CXX=g++
RM=rm -rf
CFLAGS=-Wall
LDLIBS=-pthread -lmraa -lupm-adxl345 -lupm-itg3200 -lupm-hmc5883l
FILE=moth

_OBJ=main.o gps.o log.o sonar.o imu.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

_HEADERS=shared.h log.h
HEADERS=$(patsubst %,src/%,$(_HEADERS))

$(ODIR)/%.o: $(SDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(ODIR)/%.o: $(SDIR)/%.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c -o $@ $<

$(FILE): $(OBJ)
	$(CXX) $(LDLIBS) -o $@ $^

.PHONY: clean

clean:
	$(RM) $(FILE) $(ODIR)/* $(SDIR)/*~

clean-logs:
	$(RM) ~/log/*

imu-test: tools/imuTester.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
