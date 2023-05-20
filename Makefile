BUILD_DIR ?= uart_dma
# 目标名
TARGET ?= example
# 指定使用的编译工具
PREFIX ?= arm-none-eabi-

INCLUDE_PATH = include

CFLAGS = -include include/config.h
CXXFLAGS = $(CFLAGS)

CURDIR := ${PWD}

MAKEFLAGS += --no-print-directory

export CFLAGS CXXFLAGS CURDIR INCLUDE_PATH TARGET BUILD_DIR PREFIX

.PHONY: all clean

all:
	$(MAKE) -f example/$(BUILD_DIR)/Makefile

clean:
	$(RM) -rf $(shell find -name *.o)
	$(MAKE) -f example/$(BUILD_DIR)/Makefile clean
