############################################################
# Makefile for our common static library libbs.a
#

# Name of library
TARGETS += $(LIB_DIR)/libbs.a

# Object files contained in the library
OBJECTS += ngets.o
OBJECTS += nsectimer.o
OBJECTS += DijkstraSemPOSIX.o

#################################################
# There is no need to edit below this line
#################################################

# Generate debugging symbols?
CFLAGS = -g -Wall
ARFLAGS = -c -r -s

#### Fixed variable definitions
CC = gcc
AR = ar
RM_DIR = rm -rf
MKDIR = mkdir
SHELL = /bin/bash
BIN_DIR = bin
LIB_DIR = lib

####

all: $(LIB_DIR) $(TARGETS)

#### Special build rules

$(LIB_DIR)/libbs.a : $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

ngets.o : ngets.c ngets.h
	$(CC) -c $(CFLAGS) $<

nsectimer.o : nsectimer.c nsectimer.h
	$(CC) -c $(CFLAGS) $<

SimpleSemPOSIX.o : SimpleSemPOSIX.c SimpleSem.h
	$(CC) -c $(CFLAGS) $<

DijkstraSemPOSIX.o : DijkstraSemPOSIX.c DijkstraSem.h
	$(CC) -c $(CFLAGS) $<

#### Fixed build rules
$(BIN_DIR)/% : %.c
	$(CC) $(CFLAGS) $< -o $@

$(LIB_DIR):
	$(MKDIR) -p $(LIB_DIR)

$(BIN_DIR):
	$(MKDIR) -p $(BIN_DIR)

.PHONY: clean
clean :
	$(RM_DIR) $(LIB_DIR) *.o

