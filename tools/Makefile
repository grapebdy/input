#
# tools/Makefile
#
# Copyright (C) 2020-2020 by qqliang
# Author: qqliang <grapel2013@gmail.com>
#

CROSS_COMPILE=
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++

CFLAGS += -I../include/ -c -Wall
CXXFLAGS += -I../include/ -c -Wall
LDFLAGS += -L../lib -lvirtkey -lpthread

INSTALLDIR="../release/"
TOOLSDIR=$(INSTALLDIR)/tools/

TARGET= virtkey-tools	\
	key


all: $(TARGET)

key: input.o
	+$(CC) $^ $(LDFLAGS) -o $@

virtkey-tools: virtkey.o
	+$(CC) $^ $(LDFLAGS) -o $@

%.o:%.c
	$(CC) -o $@ $(CFLAGS) $<

%.o:%.cpp
	$(CXX) -o $@ $(CXXFLAGS) $<

clean:
	@rm -fr $(TARGET) *.o

install:
	@echo "install tools...."
	@test -d $(INSTALLDIR) || mkdir $(INSTALLDIR)
	@test -d $(TOOLSDIR) || mkdir $(TOOLSDIR)
	@cp -rf $(TARGET) $(TOOLSDIR)
#	@cp -rf *.sh $(TOOLSDIR)
	$(if $(TOOLSRC), cp -rf *.c Makefile $(TOOLSDIR), )
