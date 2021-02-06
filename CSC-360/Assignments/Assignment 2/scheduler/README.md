# Assignment 2

My solutions for the assignment is complete and has no weird behavior.

## Design

### Task

A task contains a couple things to keep track of its state

* Its current state ie. running, ready, sleeping, waiting, blocked, done
* If waiting who its waiting on
* If sleeping when to wake

The state I represented by an enum with a variant for each.

The waiting on I represented as an array which gets added to every task_wait call.

The sleeping time I just add the time they want to sleep to the current time and is a size_t.

### Scheduling and updating state

When a task is first created it is put in the ready state with the exception of the main task which starts running.

Then when the task is waited for it is added to the list of tasks to be completed before the current task and switched too.
Once that task is complete it is set to done and another task is scheduled using round robin. The main task will no do this unless a define is put at the top. The scheduler goes through in a round robin fashion first updating the states of the tasks, by checking if all tasking waiting are complete or if there is user input or if the sleep is done sets the appropriate state READY or keep it the same. After updating it checks if it is READY and if it is schedules it, if it is not is moves onto the next task. If no task is READY it will check to make sure all of them are DONE stops if they are. If not all are done it waits until one becomes aviable.

### task_readchar

This relies on how the ucontext library works I make it if there is no user input avaiable then it switched to another task and sets itself to blocked. Since it will only every return to that point in execution when there is user input found in the update method we can just return what was found there.
