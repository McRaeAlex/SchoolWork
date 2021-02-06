CC=gcc
CFLAGS=-Wall -Werror -g

SEEsh: SEEsh.o
	$(CC) $(CFLAGS) -o $@ $^

SEEsh.o: SEEsh.c SEEsh.h
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: run, gdb, valgrind, clean

run: SEEsh
	./SEEsh

clean:
	rm *.o SEEsh

gdb: SEEsh
	gdb ./SEEsh

valgrind: SEEsh
	valgrind ./SEEsh --leak-check=full

