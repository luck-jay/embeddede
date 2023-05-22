# 目标CPU架构
TARGET_ARCH := cotex-m3
# 指定编译器
PREFIX := arm-none-eabi-

INCLUDE_PATH += example/time_wheel/include
INCLUDE_PATH += include/stm32f10x

CFLAGS += -DSTM32F10X_HD
# 指定CPU类型
CPU = -mcpu=cortex-m3
# 为CPU启用硬件浮点运算
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# 汇编编译选项设置
ASFLAGS += $(MCU) $(OPT) -Wall -fdata-sections -ffunction-sections
# c文件编译选项
CFLAGS += $(ASFLAGS) $(C_INCLUDES)
CXXFLAGS += $(CFLAGS)
CFLAGS += -lstdc++
# 指定链接文件
LDSCRIPT = example/time_wheel/stm32_flash_hd.ld

# 指定链接库文件
LIBS = -lc -lm -lnosys

# 指定链接选项(-specs=nosys.specs可能是解决cpp文件支持的参数)
LDFLAGS += $(MCU) -specs=nano.specs -specs=nosys.specs -T$(LDSCRIPT) $(LIBS) -Wl,-Map=$(TARGET).map,--cref -Wl,--gc-sections -lstdc++

example-y += main.o
example-y += clock.o
example-y += time_wheel.o
example-y += system_stm32f10x.o
example-y += startup_stm32f10x_hd.o

lib-y += stm32f10x/misc.o
lib-y += stm32f10x/stm32f10x_exti.o

-include scripts/Makefile.build

.PHONY: all clean

# 指定需要执行的目标
all: $(TARGET).bin

# 生产elf文件
$(TARGET).elf: $(obj-y)
	$(CC) $^ $(LDFLAGS) -o $@
	$(SZ) $@
# 生产bin文件
%.bin: %.elf
	$(CP) -O binary -S $< $@

clean:
	$(RM) -r $(TARGET).*
