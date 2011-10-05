KERNELDIR=/usr/src/linux-2.6
MAKE=make
PWD=$(shell pwd)

obj-m := lf_module.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
