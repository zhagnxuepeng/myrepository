KERNELDIR := /lib/modules/$(shell uname -r)/build

test:
	@make  -C  $(KERNELDIR)  M=$(shell  pwd)  modules

app:
	@gcc test.c -o app

clean:
	@rm -rf  *.ko  *.o  *.mod.*  *order  Module.symvers

obj-m=  chrdrvtest.o
