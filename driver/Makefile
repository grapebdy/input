#
# driver/Makefile
#
# Copyright (C) 2020-2020 by qqliang
# Author: qqliang <grapel2013@gmail.com>
#

INSTALLDIR ="../release/"
DRVDIR = $(INSTALLDIR)/driver

ifneq ($(KERNELRELEASE),)
obj-m := virtkey.o
else
KDIR :=/lib/modules/$(shell uname -r)/build/
MDIR :=$(PWD)

all:
	make -C $(KDIR) M=$(MDIR) modules

clean:
	@rm -rf *.ko *.o *.mod.o *.mod.c *.symvers  modul* .*.cmd .tmp_versions/

install:
	@echo "install drivers...."
	@test -d $(INSTALLDIR) || mkdir $(INSTALLDIR)
	@test -d $(DRVDIR) || mkdir $(DRVDIR)
	@cp -rf *.ko $(DRVDIR)
endif
