obj-m := mic_driver.o
ARCH=arm 
COMPILER=/home/etibbod/Downloads/petalinux-v2014.4-final/tools/linux-i386/arm-xilinx-linux-gnueabi/bin/arm-xilinx-linux-gnueabi-
KDIR := /home/etibbod/dipterv/linux-xlnx/

PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) SUBDIRS=$(PWD) clean
