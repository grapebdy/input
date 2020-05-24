GIT_VER=`git log --oneline | head -n 1 | cut -d ' ' -f 1`
TOOLS_VERSION=$(GIT_VER)-linux
INSTALL_DIR=virtkeyborad

VERSION=0.1

RELNAME=virtkey-utils-$(GIT_VER)
PROJ_OUT=$(PWD)/out
INSTALLDIR :=$(PROJ_OUT)/$(RELNAME)

all: build

build: driver lib tools tags

driver: FORCE
	make -C driver MDIR=$(PWD)/driver all

tools: lib FORCE
	make -C tools

lib: FORCE
	make -C lib VERSION=$(VERSION)

clean:
	make -C driver clean
	make -C tools clean
	make -C lib clean
	@rm -rf modules.order  Module.symvers

install:
	@echo "clean old release...."
	@rm -fr $(INSTALLDIR) virtkey-*.tar.gz
	@mkdir -p $(INSTALLDIR)
	@make -C driver INSTALLDIR=$(INSTALLDIR) install
	@make -C tools INSTALLDIR=$(INSTALLDIR) install
	@make -C lib INSTALLDIR=$(INSTALLDIR) install
	@make -C include INSTALLDIR=$(INSTALLDIR) install

release:
	@test -d $(INSTALL_DIR) || mkdir $(INSTALL_DIR)
	@cp -fr driver/virtkey.ko $(INSTALL_DIR)/
	@cp -fr tools/virtkey      $(INSTALL_DIR)/
	@tar cjmf virtkey-utils-$(TOOLS_VERSION).tar.gz $(INSTALL_DIR)/
	@rm -fr $(INSTALL_DIR)/

distclean: clean
	@rm -fr virtkey-*.tar.gz
	@rm -fr out tags

tags:
	@ctags -R

FORCE:
.PHONY: $(PHONY)
