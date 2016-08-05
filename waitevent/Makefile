KERNELDIR :=/lib/modules/$(shell   uname  -r)/build
#KERNELDIR :=/root/fs4412/kernel/linux-3.14-fs4412
test:
	make  -C  $(KERNELDIR)  M=$(shell  pwd)  modules



clean:
	rm  -rf *.ko  *.o   *mod*   *order     Module.symvers


obj-m=waitevent.o



