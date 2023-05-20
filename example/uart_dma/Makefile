# 目标CPU架构
TARGET_ARCH := cotex-m3

INCLUDE_PATH += example/uart_dma/include
INCLUDE_PATH += include/stm32f10x

CFLAGS += -Iarch/$(TARGET_ARCH)
CFLAGS += -DSTM32F10X_HD
# 指定CPU类型
CPU = -mcpu=cortex-m3
# 为CPU启用硬件浮点运算
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# 汇编编译选项设置
ASFLAGS += $(MCU) $(OPT) -Wall -fdata-sections -ffunction-sections
# c文件编译选项
CFLAGS += $(MCU) $(OPT) $(C_INCLUDES) -Wall -fdata-sections -ffunction-sections -lstdc++
# cpp文件编译选项
CXXFLAGS += $(MCU) $(OPT) $(C_INCLUDES) -Wall -fdata-sections -ffunction-sections
# 指定链接文件
LDSCRIPT = example/uart_dma/stm32_flash_hd.ld

# 指定链接库文件
LIBS = -lc -lm -lnosys

# 指定链接选项(-specs=nosys.specs可能是解决cpp文件支持的参数)
LDFLAGS += $(MCU) -specs=nano.specs -specs=nosys.specs -T$(LDSCRIPT) $(LIBS) -Wl,-Map=$(TARGET).map,--cref -Wl,--gc-sections -lstdc++

# 得到所有的头文件参数
CFLAGS += $(patsubst %, -I%, $(INCLUDE_PATH))

obj-y += main.o
obj-y += uart.o
obj-y += system_stm32f10x.o
obj-y += startup_stm32f10x_hd.o

obj-y := $(addprefix example/$(BUILD_DIR)/, $(obj-y))

-include arch/$(TARGET_ARCH)/Makefile

obj-y += libs/stm32f10x/misc.o
obj-y += libs/stm32f10x/stm32f10x_dma.o
obj-y += libs/stm32f10x/stm32f10x_rcc.o
obj-y += libs/stm32f10x/stm32f10x_gpio.o
obj-y += libs/stm32f10x/stm32f10x_usart.o
obj-y += libs/stm32f10x/stm32f10x_exti.o

# 指定伪目标
.PHONY: all clean

# 指定需要执行的目标
all: $(TARGET).bin

# 编译c文件
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
# 编译cpp文件
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
# 编译汇编文件
%.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@
# 生产elf文件
$(TARGET).elf: $(obj-y)
	$(CC) $(obj-y) $(LDFLAGS) -o $@
	$(SZ) $@
# 生产bin文件
$(TARGET).bin: $(TARGET).elf
	$(CP) -O binary -S $< $@

clean:
	$(RM) -rf $(TARGET).*
