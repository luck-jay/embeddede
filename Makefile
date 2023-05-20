BUILD_DIR := uart_dma

# 目标名
TARGET := example
INCLUDE_PATH = include

CFLAGS = -include include/config.h
CXXFLAGS = $(CFLAGS)

CURDIR := ${PWD}

# 指定使用的编译工具
PREFIX = arm-none-eabi-

# 判断是否添加交叉编译器路径，如果添加了交叉编译器路径，那就在添加上这个路径
ifdef GCC_PATH
CC  = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS  = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP  = $(GCC_PATH)/$(PREFIX)objcopy
SZ  = $(GCC_PATH)/$(PREFIX)size
else
CC  = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS  = $(PREFIX)gcc -x assembler-with-cpp
CP  = $(PREFIX)objcopy
SZ  = $(PREFIX)size
endif

export CC CXX AS CP SZ CFLAGS CXXFLAGS CURDIR INCLUDE_PATH TARGET BUILD_DIR
export TARGET_ARCH

.PHONY: all clean

all:
	$(MAKE) -f example/$(BUILD_DIR)/Makefile

clean:
	$(RM) -rf $(shell find -name *.o)
	$(MAKE) -f example/$(BUILD_DIR)/Makefile clean
