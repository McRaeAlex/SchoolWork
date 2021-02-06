#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t p0 = getpid();
    printf("INIT PID: %d\n", p0);
    
    fork();
    pid_t p1 = getpid();
    if (p1 != p0)
        printf("FIRST CALL: %d PARENT: %d\n", p1, p0);

    fork();
    pid_t p2 = getpid();
    if (p2 != p0 && p2 != p1)
        printf("SECOND CALL: %d PARENT: %d\n", p2, p1);

    fork();
    pid_t p3 = getpid();
    if (p3 != p0 && p3 != p2 && p3 != p1)
        printf("THIRD CALL: %d PARENT: %d\n", p3, p2);

    return 0;
}