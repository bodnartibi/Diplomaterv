obj-m := hello.o
ARCH=arm 
COMPILER=arm-linux-gnueabi-
KDIR := /home/etibbod/dipterv/linux-xlnx
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) SUBDIRS=$(PWD) clean
