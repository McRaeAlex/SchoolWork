CC := gcc
CFLAGS := -g -Wall -Wno-deprecated-declarations 

diningOut: main.c
	$(CC) $(CFLAGS) -o $@ $^ -pthread

.PHONY: run

run: diningOut
	./diningOut