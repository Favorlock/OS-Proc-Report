CONFIG_MODULE_SIG=n

obj-m += procReport.o

KDIR := /lib/modules/$(shell uname -r)/build

all:
	sudo $(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	sudo $(MAKE) -C $(KDIR) M=$(shell pwd) clean

insmod:
	# Clear messages
	sudo dmesg -C
	# Install the module
	sudo insmod procReport.ko
	# Print messages
	sudo dmesg

rmmod:
	# Clear messages
	sudo dmesg -C
	# Remove the module
	sudo rmmod procReport.ko
	# Print messages
	sudo dmesg

test:
	# Remove the module if it exists
	-sudo rmmod procReport.ko
	# Clear messages
	sudo dmesg -C
	# Install the module
	sudo insmod procReport.ko
	# Remove the module
	sudo rmmod procReport.ko
	# Print messages
	sudo dmesg
