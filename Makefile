#******************************************************************************
# Copyright (C) 2026 by Dwayne St George
#
# 2024 Mashiach Games. All rights reserved. This notice serves as a clear 
# declaration that Mashiach Games (Representing Jesus Christ), the copyright holder, 
# own's the rights to ths software and all of its contents. No part of this software
# may be modified, copied, reproduced or distributed without the permission of
# Mashiach Games.
#
#*****************************************************************************

#------------------------------------------------------------------------------
# Simple makefile for the Meteors Game build system
#
# Use: make [TARGET] [PLATFORM-OVERRIDES] (PLATFORM=x86 / PLATFORM=x64) OR
# mingw32-make.exe [TARGET] [PLATFORM-OVERRIDES] (PLATFORM=x86 / PLATFORM=x64)
#
# Build Targets:
#      build - Will build executable
#      address-map - Will build c1m2.map file
#      <FILE.o> - Will build object files
#      compile-all - Will build all object file
#      <FILE>.i - Will build preproccessor files
#      <FILE>.asm - WIll build assembly files
#      <FILE>.d - WIll build dependency files
#      clean - removes all generated files
#
# Platform Overrides:
#      CPU - HOST
#      ARCH - x86 & x64
#      SPECS - Specs file to give the linker (nosys.specs)
#
#------------------------------------------------------------------------------

# Architecture Specific Flags
LINKER_FILE = 
CPU = HOST
ARCH = x64
SPECS = 
CSTD = c99

#Compiler Flags and Defines
CC = gcc
LD = 
BASENAME = netcom
TARGET = $(BASENAME)
MAP = $(BASENAME).map
LDFLAGS = -Wl,-Map=$(MAP)
CFLAGS = -Wall -g
CPPFLAGS = -E

# Files
main_SRCS = *.c
OBJ = $(patsubst %.c, %.o, $(main_SRCS))
PP = $(patsubst %.c, %.i, $(main_SRCS))
ASM = $(main_SRCS).asm
DEP = $(patsubst %.c, DEP.d, $(main_SRCS))
COPY = ./copy.sh

# This phony of build was created to prevent any errors that may occur if there is a file named build
.PHONY : build
ifneq ($(wildcard copy.sh),)
build : $(main_SRCS)
	$(CC) $(CFLAGS) $(main_SRCS) -o $(TARGET)
	./copy.sh
else
build : $(main_SRCS)
	$(CC) $(CFLAGS) $(main_SRCS) -o $(TARGET)
endif 

# This phony of clean was created to prevent any errors that may occur if there is a file named clean
clean :
	rm -r $(TARGET)
.PHONY : clean 

