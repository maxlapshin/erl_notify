# Kernel Version Check
KVER  := $(shell uname -r | cut -c1-3 | sed 's/2\.[56]/2\.6/')
KVERSUB := $(shell uname -r | cut -c5-6)
KVER3 :=$(shell uname -r | cut -c1-1)
# Makefile for a basic kernel module - 2.4.x
KINC=
ifeq ($(KINC),)
KINC  := $(shell uname -r)
endif
CC    := gcc
MODCFLAGS := -DMODULE -D__KERNEL__ -DLINUX -DOLDKERNEL -O -I/usr/src/linux-$(KINC)/include

# Makefile for a basic kernel module - 2.6.x
obj-m := erl_notify.o
KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)

# MODULE -- This tells the header files to give the appropriate
# definitions for a kernel module.
#
# __KERNEL__ -- This tells the header files that this code will
# be run in kernel mode not as part of a user process.
#
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	$(CC) poller.c -o poller -lpthread

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean


