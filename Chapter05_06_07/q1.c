#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int x = 100;
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // Child process
        printf("Child process here (PID: %d): original value of x: %d\n", (int) getpid(), x);
        x = 300; // Modifies the original x for the child
        printf("Child process after change (PID: %d), new value of x: %d\n", (int) getpid(), x);
    } else {
        // Parent process
        printf("Parent process here (PID: %d): original value of x: %d\n", (int) getpid(), x);
        x = 200; // Modifies the original x for the parent
        printf("Parent process after change (PID: %d), new value of x: %d\n", (int) getpid(), x);
    }
    return 0;
}
