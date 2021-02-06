# CSC 360 Operating Systems

## Grading scheme

| Assignment | Percentage |
| ---------- | ---------- |
| Tutorials  | 10% |
| Assignment 1 | 8% |
| Test 1 | 12% |
| Assignment 2 | 15% |
| Test 2 | 12% |
| Assignment 3 | 18% |
| Final | 25% |

Text readings and forum questions will be part of the assignments.

# Definitions

|Term|Definition|
|----|----------|
| Affinity | Threads that do not pollute each others cache.|
| Operating system | Mediates programs access to hardware resources.|
| Page fault | Attempted to get something from memory (cache, ram) but its not there so you must go to disk.|
| System Calls | Calls to the operating system to do some task.|
| Trap | A interrupt that is called by a user space program to make a system call.|
| Virtual Machine | An operating system which runs on top of another operation system or on the same hardware as other operating systems.|
| Program | Bytes on disk which encode instructions to be run |
| Process | An instance of a program to be run |
| Process control block | Holds the state of processes running on a operating system |
| DMA | Direct memory access. This occurs when a device controller write directly to a section in main memory.|
| Race condition | When running the program without any IO or with the same IO it does different things based on the scheduler |

# Topics

## Magical Abstractions

Hardware 

* Disks
* memory 
* processor
* network
* monitor
* keyboard
* mouse

Operating System

* files
* programs
* threads of control
* communication
* windows, graphics
* input
* locator

The operating system has layers. On the bottom is the hardware, then the operatng system then the applications in userspace on top.

Privileged instructions can only be done by the operating system. This is things like managing memory, pages, tlbs loads, interfacing with network cards, interupts, etc.

System calls are generally not statically linked to avoid tying your program to a single operating system.

## Kernel Data Structures

* Singly linked list
* Doubly linked list
* Circular linked list
* Binary search trees
* Hash functions
* Bitmaps - string of n binary digits representing teh status of n items

## Transition from User to Kernel Mode

A timer runs which prevents infinite loop / process hogging resources. This is also know as preemptive scheduling.

Hardware is involved with moving from userspace to kernal space.

Trap a interupt that is called by a userspace program to make a system call. This switches mode from user to kernel mode.

### Order of operations

1. Runtime call (read(int fileDescriptor, void* buffer, int numBytes))
2. Move into kernel mode
3. Trap handler runs
    - Verify syscall
    - Find handler
4. kernel routine runs (sys_read())
    - Verify args
    - Init read
    - Choose next process to run (because we are waiting for IO on the process that called the trap)
    - Setup returns
    - Restore app state
5. Return back to user mode

Generally sys calls are made from a standard c library, this allows multiple standard libraries to be dynamically linked allowing cross platform (cross os) applications. Think posix.
On linux this is glibc.

## Virtualization

Allows multiple operating system on one machine running.
Created for servers / cloud which need to share resources securely.

### Cloud computing

EC2 for example has thousands of servers but millions of VMs.
Cloud can have many different things, like load balancers, storage, etc.. Its complex.

| Types of cloud | Description |
| Public cloud | available via internet to anyone willing to pay like AWS |
| Private cloud | run by a company for the companies own use |
| hybrid cloud | includes both public and private cloud | 

## Real time OS's

Constraint based operating systems with a single purpose. These are often for embedded systems.

## Design and implementation of Operating systems

* should be convenient to use, easy to learn, reliable, safe, and fast.
* easy to design, efficient, etc.

Sometimes these are contradictions.

Policy: What will be done?
Mechanism: How to do it?
Seperation of policy and mechanisms are important for flexibility and multiple implementations.(Maintainable)

This is often why OS's are layered.

user -> shells/commands/compilers/interpreters -> system calls interface to kernel -> kernel interface to hardware

## Micro kernels

Allows some "system level" programs to run in userspace. Things like the filesystem. This allows for things to be more modular and safe. Generally easier to port. But due to user to kernel space switch they can be slow.

## Process Management

Processes have state, and its the operating system responsibility to store the state of a process if it preempts the process. Processes can have different priority. Processes are independent from each other, this means that if one process fails others (generally) do not.

Programs are passive and stored on disk. A process is a active program.

Things in a process

* Stack which contains temporary data (func params, ret addresses, local vars)
* Data section, contains global vars
* Heap containing memory that is dynamically allocated during runtime

Operations 

* create
* delete 
* suspend
* resume
* clone
* inter-process communication
* inter-process synchronization
* create/delete child process

Note: fork is called once but returns twice. The fork returns different things for the child and parent process.(PID: Process id)

Midterm: What runs next after a fork call? Parent, Child, Both? Answer: Unknown, we have no control over the scheduler.

Exec: calls an executable as a child process.

### Process in virtual memory

| Virtual memory |
|-----|
| Stack |
| Empty space to be taken by stack or heap |
| Global vars or static data |
| Code or text area |

### Possible states of a process

* new
* running
* waiting
* ready
* terminated

Ready means that its waiting to be run and can run
Waiting means it's waiting on some IO (disk, network, etc.)

Look at lecture 5 3.9 for diagram of how the states interact.

## Threads

A process within a process. It's a process with multiple program counters which allows a single program to have multiple threads of control.

(** How does this work? **)

## Computer-System Operation

A bunch of independent systems which work with each other through known interfaces.

This allows IO devices to work with the CPU concurrently.

* Each device controller is in charge of a particular device type
* Each device controller has a local buffer
* CPU moves data between the local buffers and main memory
* Device controller informs CPU that it is done by causing an interrupt

Note: Device is a hardware device ie. GPU, network card, keyboard

DMA - Direct memory access allows a device controller to put data directly in memory. Think when a network request finished the packets get put into a buffer in main memory not through the CPU registers. This is facilitated by a interrupt from the device controller.

Midterm 1: whats the difference between a built in command and external command?
It doesn't fork.

## Schedulers

### Short-term scheduler
selects which process is next and allocates CPU for it. Sometimes this is the only scheduler

### Long-term scheduler

* selects which process should be brought into the ready queue
* generally invoked infrequently (seconds)
* determines degree of multiprogramming

#### Types of processes

IO Bound -> spends more time doing IO than computations. Many short bursts of CPU time.
CPU Bound -> spends more time doing computations. Few long bursts of CPU time.

* The long term scheduler aims for a good mix of both.

### Medium-term scheduler

* Can be added if a degree of multiple programming needs to decrease.
* Remove process from memory, store on disk, and bring back from disk. (swapping)

## Context Switch

The act of saving the state of one process to run another. This is generally costly and stores the **Context** as the PCB. (process control block)

## Midterm Review

Process Control Block handles the context for a process.

- IO bound vs CPU bound
- PCB and CPU state
    - PC, SP, registers
    - CPU contains current valuess
- Trap: execs a sys call
- Exception: Program does something unexpected
- Interrupt: A hardware device requests service

Note: remember context switch != mode switch

Context switch -> changing process
Mode switch -> User to kernel mode

if a parent is not waiting for a child the child is a zombie process
if the parent is terminated without invoking wait the process is an orphan

- Computing environments (cloud computing)

## Threads & Concurrency

Race conditions are when you don't know what the code will do due to multiple threads of control accessing the same data.

Threads share one memory space but generally create different stacks for each thread. The heap is shared.

Preemptive scheduling - The scheduler decideds how to break up the time for each task (thread, process, etc)
Cooperative scheduling - The task, process or thread decides when to stop running.

### User level threads
- Management done by a user-level library
- Pthreads
- It make map to kernel level threads but it doesn't have too
- May be scheduled by the kernel or another user-level scheduler


### Kernel threads
- Low level not avaliable to most programs

## File system

1. The operating system hides the hardware specific interface.
2. Allocates disk blocks.
3. Checks permissions.
4. Understandings the firectory file structure.
5. Maintain metadata.
6. Performance.
7. Flexibility.

There is a block list portion of the i-node. It points to blocks in the file contents area. Unix allows multiple sizes for blcoks.
There are 13 block pointers in a i-node. The first 10 are "direct pointers". The single, double and triple indirect pointers.

Seek time is aprrox. the seek distance. ie. distance from where we are currently looking to the place we want to look.

The OS maintains a queue of requests, per disk or device. The queue is not really a queue, it may do things out of order to optimize it.

### Mass Storage Systems

Magnetics disks provide the bulk of secondary storage. The have a track with data on it and a arm that reads the data.

Solid state disks remove the mechanical element from secondary storage. (Generally more reliable)


