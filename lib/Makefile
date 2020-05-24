#
# lib/Makefile
#
# Copyright (C) 2020-2020 by qqliang
# Author: qqliang <grapel2013@gmail.com>
#

CROSS_COMPILE=
CC = $(CROSS_COMPILE)gcc
CXX= $(CROSS_COMPILE)g++
AR= $(CROSS_COMPILE)ar
STRIP= $(CROSS_COMPILE)strip

CXXFLAGS += -I../include -std=c++11 -lpthread -Wall
CFLAGS += -I../include -lpthread -c -fPIC -Wall
LDFLAGS += -shared -fPIC

VERSION=0.1
INSTALLDIR="../release/"
LIBDIR=$(INSTALLDIR)/lib/

LIBOBJS := virtkeylib.o

all: libvirtkey

libvirtkey: $(LIBOBJS)
	@$(AR) -cr $@.a.$(VERSION) $^
	+$(CC) $(LDFLAGS) $^ -o $@.so.$(VERSION)
	@$(STRIP) --strip-unneeded $@.so.$(VERSION)
	@ln -sf $@.so.$(VERSION) $@.so
	@ln -sf $@.a.$(VERSION) $@.a

%.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $<

%.o: %.c
	$(CC) -o $@ $(CFLAGS) $<

install:
	@echo "install library...."
	@test -d $(INSTALLDIR) || mkdir $(INSTALLDIR)
	@test -d $(LIBDIR) || mkdir $(LIBDIR)
	@cp -fr *.a *.a.* *.so *.so.* $(LIBDIR)

clean:
	@rm -rf *.o *.a *.a.* *.so *.so.*
