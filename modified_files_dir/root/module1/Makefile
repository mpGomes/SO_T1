KERNELDIR=/usr/src/linux-2.6
MAKE=make
PWD=$(shell pwd)

obj-m := hello1.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
