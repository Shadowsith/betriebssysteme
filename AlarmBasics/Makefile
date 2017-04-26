############################################################
# General purpose makefile
#
# Shows use of libraries
# - use system library librt
# - use our own common library libbs
#
# - chain to Makefile of library in ../Common

# Please add all targets in ./bin here

TARGETS += $(BIN_DIR)/seqAlarm
TARGETS += $(BIN_DIR)/forkAlarm
TARGETS += $(BIN_DIR)/threadAlarm

# For our common library
LIBS    += ../Common/lib/libbs.a

#################################################
# There is no need to edit below this line
#################################################

# Generate debugging symbols?
CFLAGS = -g -Wall
INCLUDES = -I../Common
LDFLAGS = -L../Common/lib
LDLIBS = -lpthread -lrt -lbs

#### Fixed variable definitions
CC = gcc
RM_DIR = rm -rf
CD = cd
MKDIR = mkdir
MAKE = make
SHELL = /bin/bash
BIN_DIR = bin

####

all: $(LIBS) $(BIN_DIR) $(TARGETS)

#### Special build rules
../Common/lib/libbs.a :
	($(CD) ../Common; $(MAKE) )

#### Fixed build rules
$(BIN_DIR)/% : %.c
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ $(LDFLAGS) $(LDLIBS)

$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)

.PHONY: clean
clean :
	$(RM_DIR) $(BIN_DIR) *.o

