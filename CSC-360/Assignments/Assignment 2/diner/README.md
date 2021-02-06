# Dining Philosopher

This is my implementation for the dining philosophers problem.

## Implementation

Philosophers - These are represented by threads
Forks - Represented by mutexs because only one person can use a fork at a time

## Strategy

N - Number of diners and forks

Each person has two forks one on their left and one on their right. 
If we give each fork a value [1:N] and tell each diner they must pickup the fork with the smaller value first then larger value because 2 of the diners have the same lowest fork they will race for that fork and whichever gets it first will be able to eat first. Then when that person is done it will be released.

Below is a ascii illustration with 3 diners.
The letters's are diners and numbers are forks.

```
 A 1 B
  3 2
   C
```

The A and B will both race for the 1 initially. C will go for 2.
If B gets the 1 fork it will wait until C releases it but since C has garenteed 
access to fork 3 (because A doesn't have its first fork) it will finish eating
and release fork 2 to B.
If A gets the fork A and C may race for fork 3 but since thats the last resource
they need there will not be a deadlock and it will be released.

## Note:

If you want them to once run onces so you can read the print outs uncomment
the RUN_ONCE define at the top of the file!