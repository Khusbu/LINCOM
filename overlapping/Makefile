#!/bin/bash

CC=g++
CFLAGS= -ansi -O5 -Wall
LDFLAGS= -ansi -lm -Wall
DIRSRC= ./src/
EXEC=community convert hierarchy
OBJ1= $(DIRSRC)graph_binary.o $(DIRSRC)community.o
OBJ2= $(DIRSRC)graph.o

all: $(EXEC)

community : $(OBJ1) $(DIRSRC)main_community.o
	$(CC) -o $@ $^ $(LDFLAGS)

convert : $(OBJ2) $(DIRSRC)main_convert.o
	$(CC) -o $@ $^ $(LDFLAGS)

hierarchy : $(DIRSRC)main_hierarchy.o
	$(CC) -o $@ $^ $(LDFLAGS)

##########################################
# Generic rules
##########################################

%.o: %.cpp %.h
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o *~ $(EXEC)
	rm -f $(DIRSRC)*.o
