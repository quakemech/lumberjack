# SPDX-License-Identifier: Apache-2.0 
# ******************************************************************************
#
# @file			Makefile
#
# @brief        Makefile for lumberjack logging library
#
# @copyright 	Copyright (C) 2024 Barrett Edwards. All rights reserved.
#
# @date         Aug 2024
# @author       Barrett Edwards
#
# ******************************************************************************

CC=gcc
#CFLAGS?= -g3 -O0 -Wall -Wextra
CFLAGS?= -g3 -O0
MACROS?=-D ENABLE_LOGGING
INCLUDE_DIR?=/usr/local/include
LIB_DIR?=/usr/local/lib
INCLUDE_PATH=-I ./ -I $(INCLUDE_DIR) 
LIB_PATH=-L ./ -L $(LIB_DIR)
LIBS=
INSTALL_PATH?=/usr/local/bin
TARGET=lumberjack

all: lib$(TARGET).a testbench

testbench: testbench.c lib$(TARGET).o 
	$(CC) $^ $(CFLAGS) $(MACROS) $(INCLUDE_PATH) $(LIB_PATH) $(LIBS) -o $@ 

lib$(TARGET).a: lib$(TARGET).o
	ar rcs $@ $^

lib$(TARGET).o: $(TARGET).c $(TARGET).h
	$(CC) -c $< $(CFLAGS) $(MACROS) $(INCLUDE_PATH) -o $@ 

clean:
	rm -rf ./*.o ./*.a testbench

install: lib$(TARGET).a
	sudo cp lib$(TARGET).a $(LIB_DIR)/
	sudo cp $(TARGET).h $(INCLUDE_DIR)/

uninstall:
	sudo rm $(LIB_DIR)/lib$(TARGET).a
	sudo rm $(INCLUDE_DIR)/$(TARGET).h

# List all non file name targets as PHONY
.PHONY: all clean install uninstall

# Variables 
# $^ 	Will expand to be all the sensitivity list
# $< 	Will expand to be the frist file in sensitivity list
# $@	Will expand to be the target name (the left side of the ":" )
# -c 	gcc will compile but not try and link 
