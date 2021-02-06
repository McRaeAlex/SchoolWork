superd: doublylinkedlist.c
	gcc -std=c11 -pedantic-errors doublylinkedlist.c -o dlist

.PHONY: run, gdb, valgrind, test

run: superd
	./dlist

gdb: superd
	gdb ./dlist

valgrind: superd
	valgrind ./dlist

test: superd
	./dlist > out.txt
	diff out.txt correct.txt