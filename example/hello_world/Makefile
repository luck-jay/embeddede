example-y += main.o

-include scripts/Makefile.build

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(obj-y)
	$(CC) $< $(LDFLAGS) -o $@

clean:
	$(RM) -f $(TARGET)
