#
# include/Makefile
#
# Copyright (C) 2020-2020 by qqliang
# Author: qqliang <grapel2013@gmail.com>
#

CROSS_COMPILE=
CC = $(CROSS_COMPILE)gcc
CXX= $(CROSS_COMPILE)g++
AR= $(CROSS_COMPILE)ar

CXXFLAGS += -I../include -std=c++11 -lpthread -Wall
CFLAGS += -I../include -lpthread -c -fPIC -Wall
LDFLAGS += -shared -fPIC

INSTALLDIR="../release/"
HEADERDIR=$(INSTALLDIR)/include/

all:

install:
	@echo "install header files...."
	@test -d $(INSTALLDIR) || mkdir $(INSTALLDIR)
	@test -d $(HEADERDIR) || mkdir $(HEADERDIR)
	@cp -fr *.h $(HEADERDIR)

clean:
	@rm -rf *.o
