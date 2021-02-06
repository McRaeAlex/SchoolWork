# Question 5

## First come first serve

In first come first serve takes the requests as they come in from the queue.
It is a slow algorithm. In the worst case it could be seeking the furthest possible
distances over and over. Given a list of reads and writes it will almost always 
perform worse than other algorithms that optimize the seek distances.

### Advantages
- All jobs are treated equal meaning no job will be untouched.

### Disadvantage
- Slow
- Does not optimize seek time

## Shortest seek time first

Calculated the shortest seek time in the queue and runs that job.

### Advantages
- Faster than FCFS on average
- Amount of jobs processed is more

### Disadvantages
- Overhead of calculating
- Some jobs may never be closest
- Jobs may get vastly different response times.

