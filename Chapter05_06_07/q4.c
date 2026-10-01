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
        // Child process transforms into /bin/ls using execvp
        char *myargs[3];
        myargs[0] = "/bin/ls"; // program to run
        myargs[1] = ".";       // argument: current directory
        myargs[2] = NULL;      // null terminator required
        
        execvp(myargs[0], myargs);
        
        // If exec fails, this line will run
        fprintf(stderr, "exec failed\n");
        exit(1);
    } else {
        // Parent waits for child process to complete
         wait(NULL);
        printf("Parent process (PID: %d)\n", (int) getpid());
    }
    
    return 0;
}
