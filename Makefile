CONFIG_MODULE_SIG=n

obj-m += procReport.o

KDIR := /lib/modules/$(shell uname -r)/build

all:
	sudo $(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	sudo $(MAKE) -C $(KDIR) M=$(shell pwd) clean
