CC := gcc
CFLAGS := -g -Wall

all: l1


.PHONY: clean run

clean:
	rm -rf li li.dSYM

run: l1
	./l1

l1: l1.c 
	$(CC) $(CFLAGS) -o $@ $^ -pthread
