#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "scheduler.h"

void task1_fn() {
  printf("I am task 1.\n");
}

void task2_fn() {
  printf("I am task 2.\n");
}

int main() {
  scheduler_init();
  
  printf("All done!\n");
  
  return 0;
}

