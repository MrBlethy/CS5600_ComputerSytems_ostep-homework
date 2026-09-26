#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int iterations = 100000; // Number of ping-pongs
    
    // 1. Lock (pin) the parent process to CPU Core 0
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    if (sched_setaffinity(0, sizeof(set), &set) < 0) {
        perror("sched_setaffinity failed");
        exit(1);
    }

    // 2. Create two UNIX pipes for back-and-forth communication
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        perror("pipe failed");
        exit(1);
    }

    // 3. Fork into Parent and Child processes
    int rc = fork();
    if (rc < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (rc == 0) {
        // --- CHILD PROCESS (Worker B) ---
        // Lock child to the exact same CPU core (Core 0)
        sched_setaffinity(0, sizeof(set), &set);

        char buf;
        for (int i = 0; i < iterations; i++) {
            read(pipe1[0], &buf, 1);    // Wait for data from parent
            write(pipe2[1], &buf, 1);   // Ping back response to parent
        }
        exit(0);
    } 
    else {
        // --- PARENT PROCESS (Worker A) ---
        char buf = 'x';
        struct timeval start, end;

        // Start stopwatch
        gettimeofday(&start, NULL);

        // The Ping-Pong Loop
        for (int i = 0; i < iterations; i++) {
            write(pipe1[1], &buf, 1);   // Send data to child (forces switch to Child)
            read(pipe2[0], &buf, 1);    // Wait for child response (forces switch back to Parent)
        }

        // Stop stopwatch
        gettimeofday(&end, NULL);
        wait(NULL); // Wait for child to finish

        // Calculate total time in microseconds
        long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
        long total_usec = (seconds * 1000000) + microseconds;

        // Math: Each single loop iteration does TWO context switches 
        // (Parent -> Child, then Child -> Parent). So we multiply iterations by 2.
        float avg_cost = (float) total_usec / (iterations * 2);

        // Print results
        printf("Total time for %d round-trips: %ld microseconds\n", iterations, total_usec);
        printf("Average cost of one context switch: %.3f microseconds\n", avg_cost);
    }

    return 0;
}
