#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main( int argc, char*argv[]){
    int iterations = 1000000;
    struct timeval start, end;

    //start the stopwatch
    gettimeofday(&start, NULL);

    // call Null sysCall repeatedly
    for (int i = 0; i < iterations; i++){
        read(0, NULL, 0);
    }

    // stop the timer 
    gettimeofday(&end, NULL);

    // Get time elapsed in microsec
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    long time_elapsed_ms = (seconds * 1000000) + microseconds;

    // Average cost for a system call 
    float avg_cost = (float) time_elapsed_ms / iterations;
   

    // Print time 
    printf("Total time elapsed for %d iterations: %ld microseconds\n", iterations, time_elapsed_ms);
    printf("The average cost of one system call : %.3f  microseconds\n", avg_cost);
    
    return 0;  
}
