BUILD_DIR ?= hello_world
# 目标名
TARGET ?= $(BUILD_DIR)

INCLUDE_PATH = include

CFLAGS = -include include/config.h
CXXFLAGS = $(CFLAGS)

CURDIR := ${PWD}

MAKEFLAGS += --no-print-directory

export CFLAGS CXXFLAGS CURDIR INCLUDE_PATH TARGET BUILD_DIR

.PHONY: all clean help

all:
ifneq ($(BUILD_DIR),)
	@$(MAKE) -f example/$(BUILD_DIR)/Makefile
else
	@echo "Please specify the compilation example, eg:\nmake BUILD_DIR=uart_dma"
endif # ifneq ($(BUILD_DIR),)

clean:
ifneq ($(BUILD_DIR),)
	$(RM) -r $(shell find -name *.o)
	@$(MAKE) -f example/$(BUILD_DIR)/Makefile clean
else
	@echo "Please specify the compilation example, eg:\nmake BUILD_DIR=uart_dma"
endif # ifneq ($(BUILD_DIR),)

help:
	@echo make [BUILD_DIR=xx] [TARGET=xxx] [PREFIX=xxx]
	@echo
	@echo BUILD_DIR: Specify the directory name under the 'example' directory.
	@echo
	@echo TARGET: Specify the file name generated after compilation.
	@echo
	@echo PREFIX: Specify the compiler to use, using arm none eabi gcc by default.
