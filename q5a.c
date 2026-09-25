#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int rc = fork();
    
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // Child process branch
        printf("Child says : Hello!(PID: %d) \n", (int) getpid());
    } else {
        // Parent process branch
        int parent_wc = wait(NULL);
        printf("Parent (PID: %d), wait() returned PID: %d\n", 
               (int) getpid(),  parent_wc);
    }
    
    return 0;
}
