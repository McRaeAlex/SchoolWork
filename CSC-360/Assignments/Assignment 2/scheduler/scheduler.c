#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

#include "scheduler.h"

#include <assert.h>
#include <curses.h>
#include <ucontext.h>

/* *************
 * OWN FUNCTIONS
 * *************
 */
bool task_runnable(task_t task);

task_t next_valid_task();

void run_next_valid_task();

#include "util.h"

// This is an upper limit on the number of tasks we can create.
#define MAX_TASKS 128

// This is the size of each task's stack memory
#define STACK_SIZE 65536

// This struct will hold the all the necessary information for each task
typedef struct task_info {
    // This field stores all the state required to switch back to this task
    ucontext_t context;

    // This field stores another context. This one is only used when the task
    // is exiting.
    ucontext_t exit_context;

    // TODO: Add fields here so you can:
    //   a. Keep track of this task's state.
    enum { READY, RUNNING, SLEEPING, WAITING, BLOCKED, DONE } state;
    //   b. If the task is sleeping, when should it wake up?
    size_t wake_time;
    //   c. If the task is waiting for another task, which task is it waiting
    //   for?
    size_t num_tasks_waiting_for;
    task_t *task_sync;
    //   d. Was the task blocked waiting for user input? Once you successfully
    //      read input, you will need to save it here so it can be returned.
    // TODO: Some buffer to store user input
    int user_input;

} task_info_t;

int current_task = 0;         //< The handle of the currently-executing task
int num_tasks = 1;            //< The number of tasks created so far
task_info_t tasks[MAX_TASKS]; //< Information for every task

// This will return true if all the tasks are complete
bool all_tasks_done() {
    for (task_t i = 0; i < num_tasks; i++) {
        if (tasks[i].state != DONE) {
            return false;
        }
    }
    return true;
}

void main_wait() {
    tasks[0].state = DONE;
    while (all_tasks_done() == false) {
        run_next_valid_task();
    }
}

/**
 * Initialize the scheduler. Programs should call this before calling any other
 * functiosn in this file.
 */
void scheduler_init() {

    // we need to save the current context into the first task
    task_info_t *main_task = &tasks[current_task];
    main_task->state = RUNNING;

    // Note the program will not go back to this point again, we are simpily
    // initing it to something so it doesn't segfault on a swap. (because the
    // missing stack).
    getcontext(&main_task->context);

    main_task->num_tasks_waiting_for = 0;
    main_task->task_sync = malloc(0);

    #ifdef MAIN_WAIT_FOR_ALL
    atexit(main_wait);
    #endif
}

/**
 * This function will execute when a task's function returns. This allows you
 * to update scheduler states and start another task. This function is run
 * because of how the contexts are set up in the task_create function.
 */
void task_exit() {
    task_info_t *finished_task_info = &tasks[current_task];
    finished_task_info->state = DONE;

    free(finished_task_info->task_sync); // Because we are single thread we can do this

    // DEBUG:
    // printf("Task: %d has exited\n", current_task);

    // Schedule the next task
    run_next_valid_task();
}

/**
 * Create a new task and add it to the scheduler.
 *
 * \param handle  The handle for this task will be written to this location.
 * \param fn      The new task will run this function.
 */
void task_create(task_t *handle, task_fn_t fn) {
    // Claim an index for the new task
    int index = num_tasks;
    num_tasks++;

    // Set the task handle to this index, since task_t is just an int
    *handle = index;

    // We're going to make two contexts: one to run the task, and one that runs
    // at the end of the task so we can clean up. Start with the second

    // First, duplicate the current context as a starting point
    getcontext(&tasks[index].exit_context);

    // Set up a stack for the exit context
    tasks[index].exit_context.uc_stack.ss_sp = malloc(STACK_SIZE);
    tasks[index].exit_context.uc_stack.ss_size = STACK_SIZE;

    // Set up a context to run when the task function returns. This should call
    // task_exit.
    makecontext(&tasks[index].exit_context, task_exit, 0);

    // Now we start with the task's actual running context
    getcontext(&tasks[index].context);

    // Allocate a stack for the new task and add it to the context
    tasks[index].context.uc_stack.ss_sp = malloc(STACK_SIZE);
    tasks[index].context.uc_stack.ss_size = STACK_SIZE;

    // Now set the uc_link field, which sets things up so our task will go to
    // the exit context when the task function finishes
    tasks[index].context.uc_link = &tasks[index].exit_context;

    // And finally, set up the context to execute the task function
    makecontext(&tasks[index].context, fn, 0);

    // INIT THE STATE OF THE CONTEXT
    tasks[index].num_tasks_waiting_for = 0;
    tasks[index].task_sync = malloc(0);
}

// This runs a task but does not tell the current task to wait for it
void task_run(task_t handle) {
    // Get the current task
    task_info_t *current_task_info = &tasks[current_task];

    // Get the new task
    task_info_t *task = &tasks[handle];

    // Make the new tasks the current
    current_task = handle;
    // DEBUG: printf("curr task: %d\n", current_task);

    // If the current_task can be run do it
    if (task_runnable(current_task)) {
        task->state = RUNNING;
        swapcontext(&current_task_info->context, &task->context);
    } else {
        // (else) after run some other task
        run_next_valid_task();
    }
}

/**
 * Wait for a task to finish. If the task has not yet finished, the scheduler
 * should suspend this task and wake it up later when the task specified by
 * handle has exited.
 *
 * \param handle  This is the handle produced by task_create
 */
void task_wait(task_t handle) {
    // TODO: Block this task until the specified task has exited.
    // Get the context we want to exec
    task_info_t *current_task_info = &tasks[current_task];
    task_info_t tas = tasks[handle];
    if (tas.state != DONE) {

        // Set it's status to waiting
        current_task_info->state = WAITING;

        // Add the handle to the list of tasks its waiting for
        current_task_info->num_tasks_waiting_for++;
        current_task_info->task_sync =
            realloc(current_task_info->task_sync,
                    current_task_info->num_tasks_waiting_for * sizeof(task_t));
        current_task_info
            ->task_sync[current_task_info->num_tasks_waiting_for - 1] = handle;

        task_run(handle);
    }
}

/**
 * The currently-executing task should sleep for a specified time. If that time
 * is larger than zero, the scheduler should suspend this task and run a
 * different task until at least ms milliseconds have elapsed.
 *
 * \param ms  The number of milliseconds the task should sleep.
 */
void task_sleep(size_t ms) {

    // TODO: Block this task until the requested time has elapsed.
    // Hint: Record the time the task should wake up instead of the time left
    // for it to sleep. The bookkeeping is easier this way.
    task_info_t *current_task_info = &tasks[current_task];

    // Set the time we should wake up at
    current_task_info->wake_time = time_ms() + ms;
    current_task_info->state = SLEEPING;

    // Get the next task
    run_next_valid_task();
}

/**
 * Read a character from user input. If no input is available, the task should
 * block until input becomes available. The scheduler should run a different
 * task while this task is blocked.
 *
 * \returns The read character code
 */
int task_readchar() {
    // TODO: Block this task until there is input available.
    // To check for input, call getch(). If it returns ERR, no input was
    // available. Otherwise, getch() will returns the character code that was
    // read.

    // Try to get the char
    // if it succseds return it right away.
    // if it fails set the task state to blocked on stdin
    //     and schedule another task
    // (in update task it should check if there is input now and schedule the
    // task if there is)
    int ch = getch();
    if (ch != ERR) {
        return ch;
    } else {
        tasks[current_task].state = BLOCKED;
        run_next_valid_task();
        // TODO: Check that it returns here
        ch = tasks[current_task].user_input;
        tasks[current_task].user_input = ERR;
        return ch;
    }

    return ERR;
}

void update_task(task_t task) {
    task_info_t *t = &tasks[task];
    switch (t->state) {
    case WAITING:
        for (size_t i = 0; i < t->num_tasks_waiting_for; i++) {
            if (tasks[t->task_sync[i]].state != DONE) {
                return;
            }
        }
        // If all the tasks we are waiting for a done then we can run this task
        t->state = READY;
        break;
    case SLEEPING:
        if (t->wake_time < time_ms()) {
            t->state = READY;
        }
        break;
    case BLOCKED: {
        int input = getch();
        if (input != ERR) {
            t->user_input = input;
            t->state = READY;
        }
        break;
    }
    default:
        break;
    }
}

bool task_runnable(task_t task) {
    // Update the task information
    update_task(task);

    // Get the task info
    task_info_t task_info = tasks[task];

    // Make sure we only run ready tasks
    return (task_info.state == READY);
}

// next_valid_task goes through the tasks and finds the next valid task to run
task_t next_valid_task() {
    while (all_tasks_done() == false) {
        for (task_t i = 0; i < num_tasks - 1; i++) {
            task_t next_task =
                ((current_task + i + 1) < num_tasks) ? current_task + i + 1 : i;
            if (task_runnable(next_task) == true) {
                return next_task;
            }
        }
    }
    return -1;
}

void run_next_valid_task() {
    task_t next_task = next_valid_task();
    if (next_task == -1) {
        return;
    }
    task_run(next_task);
}