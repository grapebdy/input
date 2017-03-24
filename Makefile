all: input
input: clean
	gcc input.c -o key
clean:
	rm -fr key
