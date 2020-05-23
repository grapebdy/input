GIT_VER=`git log --oneline | head -n 1 | cut -d ' ' -f 1`
TOOLS_VERSION=$(GIT_VER)-linux
INSTALL_DIR=virtkeyborad

all: build

build:
	make -C driver MDIR=$(PWD)/driver all
	make -C tools

clean:
	make -C driver clean
	make -C tools clean
	rm -rf modules.order  Module.symvers

release:
	@test -d $(INSTALL_DIR) || mkdir $(INSTALL_DIR)
	@cp -fr driver/virtkey.ko $(INSTALL_DIR)/
	@cp -fr tools/virtkey      $(INSTALL_DIR)/
	@tar cjmf virtkey-utils-$(TOOLS_VERSION).tar.gz $(INSTALL_DIR)/
	@rm -fr $(INSTALL_DIR)/

distclean: clean
	@rm -fr virtkey-*.tar.gz
