# Question 4

In part 1 in physical memory is 2 page tables for for each of the processes. Then a heap and stack for each of the processes.
This is different than in part 2 where there is only one page table for both threads and one heap. Each thread has a stack but when exec is called the process is replaced with a new stack, heap, and page table.