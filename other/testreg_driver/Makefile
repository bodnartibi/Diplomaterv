obj-m := testreg.o
#ARCH=arm 
#COMPILER=/home/etibbod/Downloads/petalinux-v2014.4-final/tools/linux-i386/arm-xilinx-linux-gnueabi/bin/arm-xilinx-linux-gnueabi-
#KDIR := /home/etibbod/dipterv/linux-xlnx

#test on my PC
KDIR=/usr/src/linux-headers-`uname -r`/

#

#PWD := $(shell pwd)
#default:
#	$(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) SUBDIRS=$(PWD) modules
#clean:
#	$(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) SUBDIRS=$(PWD) clean

PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean

