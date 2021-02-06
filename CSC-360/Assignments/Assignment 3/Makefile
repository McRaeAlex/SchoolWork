CC:=gcc
CFLAGS:=-g -Wall -Werror -std=c11 -pedantic -O0

TESTFILES := $(wildcard apps/test*.c) 
$(info TESTFILES are $(TESTFILES))

TESTS := $(TESTFILES:apps/%.c=%)
$(info TESTS are $(TESTS))

all: $(TESTS) disk.o file.o

test%: apps/test%.c file.o disk.o
	$(CC) $(CFLAGS) -o apps/$@ $^

disk.o: disk/disk.c disk/disk.h
	$(CC) $(CFLAGS) -c -o $@ $<

file.o: io/File.c io/File.h disk.o
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -rf *.o
	find apps -type f -not -name '*.c' -print0 | xargs -0 rm --
