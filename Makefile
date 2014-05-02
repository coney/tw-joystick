TARGET=tw-joystick
TARGET_BIN=$(TARGET).ko
obj-m += $(TARGET).o
$(TARGET)-objs := joystick.o js_proc.o js_dev.o js_input_gpio.o
EXTRA_CFLAGS += -std=gnu99 -Wno-declaration-after-statement 

all:
	make -C $(KERNEL_SRC) M=$(PWD) modules

clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean
	rm -rfv *.mod.c *.mod.o *.o *.ko modules.order

scp:
	scp $(TARGET_BIN) $(DEVICE_SCP_PATH)
