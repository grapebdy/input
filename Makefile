ifneq ($(KERNELRELEASE),)
obj-m := input.o
else
KDIR :=/home/mprc/linux-3.19/
#KDIR :=/home/mprc/workspace/P86/working/linux-3.10/
all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	@rm -rf *.ko *.o *.mod.o *.mod.c *.symvers  modul* .*.cmd .tmp_versions/

endif
