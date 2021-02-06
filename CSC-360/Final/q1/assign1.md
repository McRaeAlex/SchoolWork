# Question 1

I learned about the different states of processes, ie. NEW, READY, RUNNING, WAITING, TERMINATED, and how to syncronize them (waitpid).
I also learned about the fork system call and how it returns different values to the calling thread and created (child) thread.

The idea that processes have seperate memory, this means if one child process fails the other processes generally do not.
Processes run independent from eachother (concurrent) and the order which they run is not known.
Processes can have different prioreties.

Processes have state which is stored in the process control block, this is things like page tables, open files, process state, the register state and other OS dependent things. This allows processes to be run concurrently by the operating system using context switches.