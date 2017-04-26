############################################################
# General purpose makefile
#
# Uses additional flags for controlling stack usage
#
# Works for all simple C-projects where
# - binaries are compiled into sub-dir bin
# - binaries are created from a single c-source of the same name
#
# Note: multiple targets (binaries) in ./bin are supported
#

# Please add all targets in ./bin here

TARGETS += $(BIN_DIR)/highlevel
TARGETS += $(BIN_DIR)/lowlevel
TARGETS += $(BIN_DIR)/do_getpid32
TARGETS += $(BIN_DIR)/do_getpid64
TARGETS += $(BIN_DIR)/sizeof
TARGETS += $(BIN_DIR)/p1
TARGETS += $(BIN_DIR)/p2
TARGETS += $(BIN_DIR)/virtualMemory1

#################################################
# There is no need to edit below this line
#################################################

# Generate debugging symbols?
# Use special flags for explicit stack usage
CFLAGS = -g -static -mpush-args -mno-accumulate-outgoing-args -Wall

#### Fixed variable definitions
CC = gcc
RM_DIR = rm -rf
MKDIR = mkdir
SHELL = /bin/bash
BIN_DIR = bin

####

all: $(BIN_DIR) $(TARGETS)

#### Fixed build rules
$(BIN_DIR)/% : %.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)

.PHONY: clean
clean :
	$(RM_DIR) $(BIN_DIR)

