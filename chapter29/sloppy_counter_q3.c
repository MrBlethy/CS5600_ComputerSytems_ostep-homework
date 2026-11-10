#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUMCPU 2
#define ITERATIONS 1000000

// Counter structure with global and local counters
typedef struct __counter_t {
    int value;                       // Global counter
    pthread_mutex_t lock;            // Global mutex

    int local[NUMCPU];                // Local counters
    pthread_mutex_t llock[NUMCPU];    // Local mutexes

    int threshold;
} counter_t;


// Initialize the counter and mutexes
void init(counter_t *c, int threshold) {
    c->value = 0;
    c->threshold = threshold;

    pthread_mutex_init(&c->lock, NULL);

    for (int i = 0; i < NUMCPU; i++) {
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}


// Increment a local counter
void increment(counter_t *c) {

    // Map the current thread to a local counter
    // Linux-specific shortcut; does not guarantee balanced mapping
    int cpu = (unsigned long)pthread_self() % NUMCPU;

    pthread_mutex_lock(&c->llock[cpu]);

    c->local[cpu]++;

    // Transfer local count to global counter at threshold
    if (c->local[cpu] >= c->threshold) {

        pthread_mutex_lock(&c->lock);

        c->value += c->local[cpu];

        pthread_mutex_unlock(&c->lock);

        c->local[cpu] = 0;
    }

    pthread_mutex_unlock(&c->llock[cpu]);
}


// Worker thread
void *worker(void *arg) {

    counter_t *c = (counter_t *)arg;

    for (int i = 0; i < ITERATIONS; i++) {
        increment(c);
    }

    return NULL;
}


// Main function
int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Usage: %s <threads> <threshold>\n", argv[0]);
        return 1;
    }

    int nums_threads = atoi(argv[1]);
    int threshold = atoi(argv[2]);

    if (nums_threads <= 0 || threshold <= 0) {
        fprintf(stderr, "Threads and threshold must be positive.\n");
        return 1;
    }

    pthread_t threads[nums_threads];

    counter_t counter;
    init(&counter, threshold);

    struct timeval start, end;

    gettimeofday(&start, NULL);

    // Create worker threads
    for (int i = 0; i < nums_threads; i++) {
        pthread_create(&threads[i], NULL, worker, &counter);
    }

    // Wait for all workers
    for (int i = 0; i < nums_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);

    double elapsed =
        (end.tv_sec - start.tv_sec) +
        (end.tv_usec - start.tv_usec) / 1000000.0;

    // Exact final count includes pending local increments
    long long total = counter.value;

    for (int i = 0; i < NUMCPU; i++) {
        total += counter.local[i];
    }

    printf("Threads: %d\n", nums_threads);
    printf("Threshold: %d\n", threshold);
    printf("Global counter: %d\n", counter.value);
    printf("Final count: %lld\n", total);
    printf("Elapsed time: %.6f seconds\n", elapsed);

    // Destroy mutexes
    pthread_mutex_destroy(&counter.lock);

    for (int i = 0; i < NUMCPU; i++) {
        pthread_mutex_destroy(&counter.llock[i]);
    }

    return 0;
}
