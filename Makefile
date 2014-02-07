obj-m += tw-joystick.o
tw-joystick-objs := joystick.o js_proc.o js_dev.o

all:
	make -C $(KERNEL_SRC) M=$(PWD) modules

clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean
	rm -rfv *.mod.c *.mod.o *.o *.ko modules.order

