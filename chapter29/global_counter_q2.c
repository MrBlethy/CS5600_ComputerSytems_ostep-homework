#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Counter structure implemented with a lock
typedef struct __counter_t {
    int value;
    pthread_mutex_t lock;
} counter_t;

// Initialize the counter and mutex
void init(counter_t *c) {
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

// Increment the shared counter
void increment(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

// Worker function executed by each thread
void *worker(void *arg) {
    counter_t *c = arg;

    for (int i = 0; i < 1000000; i++) {
        increment(c);
    }

    return NULL;
}

int main(int argc, char *argv[]) {

    // Check command-line arguments
    if (argc != 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    // Define number of threads
    int nums_threads = atoi(argv[1]);

    if (nums_threads <= 0) {
        fprintf(stderr, "Number of threads must be positive\n");
        return 1;
    }

    printf("Number of workers: %d\n", nums_threads);

    // Declare and initialize the shared counter
    counter_t counter;
    init(&counter);

    // Declare thread identifiers
    pthread_t threads[nums_threads];

    // Declare timing variables
    struct timeval start, end;

    // Start measuring execution time
    gettimeofday(&start, NULL);

    // Create threads and pass the shared counter address
    for (int i = 0; i < nums_threads; i++) {
        if (pthread_create(&threads[i], NULL, worker, &counter) != 0) {
            fprintf(stderr, "Failed to create thread %d\n", i);
            return 1;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < nums_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Stop measuring execution time
    gettimeofday(&end, NULL);

    // Calculate elapsed time in seconds
    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Elapsed time: %.6f seconds\n", elapsed);
    printf("Final counter: %d\n", counter.value);

    // Clean up mutex
    pthread_mutex_destroy(&counter.lock);

    return 0;
}

