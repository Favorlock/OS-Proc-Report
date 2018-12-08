CONFIG_MODULE_SIG=n

obj-m += procReport.o

KDIR := /lib/modules/$(shell uname -r)/build

all:
	sudo $(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	sudo $(MAKE) -C $(KDIR) M=$(shell pwd) clean

insmod:
	sudo dmesg -C
	sudo insmod procReport.ko
	sudo dmesg

rmmod:
	sudo dmesg -C
	sudo rmmod procReport.ko
	sudo dmesg

test:
	sudo dmesg -C
	sudo insmod procReport.ko
	sudo rmmod procReport.ko
	sudo dmesg
