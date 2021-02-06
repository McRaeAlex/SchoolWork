
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>


// int main()
// {
// pid_t pid;

// 	/* fork a child process */
// 	pid = fork();

// 	if (pid < 0) { /* error occurred */
// 		fprintf(stderr, "Fork Failed\n");
// 		return 1;
// 	}
// 	else if (pid == 0) { /* child process */
// 		printf("I am the child %d\n",pid);
// 		execlp("/bin/ls","ls",NULL);
// 	}
// 	else { /* parent process */
// 		/* parent will wait for the child to complete */
// 		printf("I am the parent %d\n",pid);
// 		wait(NULL);
		
// 		printf("Child Complete\n");
// 	}
    
//     return 0;
// }

void* fn() {
    pthread_t self = pthread_self();
    printf("I am the child %d\n", self);
    execlp("/bin/ls","ls",NULL);
}

int main() {
    pthread_t self = pthread_self();
    pthread_t tid;
    if(pthread_create(&tid, NULL, fn, NULL)) {
        fprintf(stderr, "Create Failed\n");
		return 1;
    }

    printf("I am the parent %d\n", self);
    if (pthread_join(tid, NULL)) {
        fprintf(stderr, "Failed to join\n");
        return 1;
    }
	
    printf("Child Complete\n");

    return 0;
}