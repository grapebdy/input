all:
	make -C driver all
	make -C test
clean:
	make -C driver clean
	make -C test clean
