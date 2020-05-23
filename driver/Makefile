ifneq ($(KERNELRELEASE),)
obj-m := virtkey.o
else
KDIR :=/lib/modules/$(shell uname -r)/build/
MDIR :=$(PWD)

all:
	make -C $(KDIR) M=$(MDIR) modules

clean:
	@rm -rf *.ko *.o *.mod.o *.mod.c *.symvers  modul* .*.cmd .tmp_versions/

endif

