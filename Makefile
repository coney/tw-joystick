obj-m += tw-joystick.o
tw-joystick-objs := joystick.o js_proc.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rfv *.mod.c *.mod.o *.o *.ko modules.order

