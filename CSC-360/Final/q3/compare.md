# Question 3

Both are concurrent but threads are more lightweight and we expect them to start faster. Processes are more
secure (lack of shared memory) but slow to start. Threads share memory whereas processes generally do not.
We generally expect the threaded version to be faster.

I made the assumption that we were still to use the exec call which means that
all the threads were going to terminate when execlp is called. This has the 
effect that the parents printf call may not run as the new thread may execute before it.
