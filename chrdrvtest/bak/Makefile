KERNELDIR := /lib/modules/$(shell uname -r)/build/
#KERNELDIR := /home/simon/tools/linux-4.6/

obj-m += chrdrvtest1.o


all:
	${MAKE} -C ${KERNELDIR} M=$(shell pwd) modules

clean:
	rm -rf *.o Module.order Module.symvers  *.ko  *.mod.*  modules.order

