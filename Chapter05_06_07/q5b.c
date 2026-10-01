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
        // Child calls wait()
        int wc = wait(NULL);
        printf("Child (PID: %d) wait() returned:  %d\n", (int) getpid(), wc);
    } else {
        // Parent waits for child so parent doesn't exit early
        sleep(1);
    }
    
    return 0;
}
