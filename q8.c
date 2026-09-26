#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int pipefd[2];

    // 1. Create the pipe
    if (pipe(pipefd) < 0) {
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    // 2. Fork Child 1 (The Writer)
    int rc1 = fork();
    if (rc1 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc1 == 0) {
        // Child 1: We want its output to go into the pipe instead of the screen
        close(pipefd[0]);                       // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO);         // Redirect stdout to pipe write-end
        close(pipefd[1]);                       // Close original descriptor

        // Run a command like "ls"
        char *args[] = {"ls", NULL};
        execvp(args[0], args);
        perror("exec failed");
        exit(1);
    }

    // 3. Fork Child 2 (The Reader)
    int rc2 = fork();
    if (rc2 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc2 == 0) {
        // Child 2: We want its input to come from the pipe instead of the keyboard
        close(pipefd[1]);                       // Close unused write end
        dup2(pipefd[0], STDIN_FILENO);          // Redirect stdin to pipe read-end
        close(pipefd[0]);                       // Close original descriptor

        // Run a command that reads from stdin, like "wc -l" (word count lines)
        char *args[] = {"wc", "-l", NULL};
        execvp(args[0], args);
        perror("exec failed");
        exit(1);
    }

    // 4. Parent closes its pipe file descriptors and waits for both children
    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}
