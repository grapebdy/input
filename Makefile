GIT_VER=`git log --oneline | head -n 1 | cut -d ' ' -f 1`
TOOLS_VERSION=$(shell date +%Y%m%d)-rl-$(GIT_VER)
INSTALL_DIR=virtkeyborad


all:
	make -C driver MDIR=$(PWD)/driver all
	make -C tools

clean:
	make -C driver clean
	make -C tools clean
	rm -rf modules.order  Module.symvers

ready:
	@test -d $(INSTALL_DIR) || mkdir $(INSTALL_DIR)

release: ready
	@cp -fr driver/virtkey.ko $(INSTALL_DIR)/
	@cp -fr tools/virtkey      $(INSTALL_DIR)/
	@tar cmvf virtkey-$(TOOLS_VERSION).tar $(INSTALL_DIR)/
	@rm -fr $(INSTALL_DIR)/

distclean:
	@rm -fr virtkey-*.tar
