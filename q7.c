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
        // Close standard output
        close(STDOUT_FILENO);
	printf("child: Hi!");
        
        // Calling printf() will fail silently because file descriptor 1 (stdout) is closed.
        printf("Where will be printed out to?.\n");
    } else {
        printf("Parent: Hi\n");
    }
    
    return 0;
}
