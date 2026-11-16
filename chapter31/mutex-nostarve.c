#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include "common_threads.h"

typedef struct __ns_mutex_t {
    sem_t lock;          // Protects the waiting counter
    sem_t lobby;         // Gate for joining the group
    sem_t area0;         // Gate to the critical section
    int waiters_count;   // Threads waiting to join the group
    int now_serving;     // Group members still waiting to enter
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
    m->waiters_count = 0;
    m->now_serving = 0;

    sem_init(&m->lock, 0, 1);
    sem_init(&m->lobby, 0, 1);
    sem_init(&m->area0, 0, 0);
}

void ns_mutex_acquire(ns_mutex_t *m) {
    // Register my arrival.
    sem_wait(&m->lock);
    m->waiters_count++;
    sem_post(&m->lock);

    // Pass the lobby gate and join the group.
    sem_wait(&m->lobby);
    m->now_serving++;

    // Remove myself from those still waiting to join.
    sem_wait(&m->lock);
    m->waiters_count--;

    if (m->waiters_count == 0) {
        // Group ready: keep lobby closed and supply the first key.
        sem_post(&m->lock);
        sem_post(&m->area0);
    } else {
        // Let another waiting thread join the group.
        sem_post(&m->lock);
        sem_post(&m->lobby);
    }

    // Take the key, or wait until it becomes available.
    sem_wait(&m->area0);
    m->now_serving--;
}

void ns_mutex_release(ns_mutex_t *m) {
    if (m->now_serving == 0) {
        // Group finished: allow the next group to form
        printf("Group finished! reopening lobby...\n");
        sem_post(&m->lobby);
    } else {
        // Pass the key to another member of this group.
        printf("Passing Area 0 key to next member...\n");
        sem_post(&m->area0);
    }
}

int loops;
int value = 0;
ns_mutex_t mutex;

void *worker(void *arg) {
    int id = *(int*)arg;
    int i;

    for (i = 0; i < loops; i++) {
        printf("Thread %d requesting access\n",id );
        ns_mutex_acquire(&mutex);
        
        // Critical section.
        printf("Access Granted\n");
        printf("Printing inside critical section\n");
        value++;
        printf("value %d\n", value);
        printf("Leaving critical section\n");

        ns_mutex_release(&mutex);
    }
    printf("Thread %d completed all iterations\n", id);
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 3);

    int num_threads = atoi(argv[1]);
    loops = atoi(argv[2]);
    assert(num_threads > 0 && loops > 0);

    pthread_t t[num_threads];
    int ids[num_threads];

    ns_mutex_init(&mutex);
    printf("parent: begin\n");

    int i;

    for (i = 0; i < num_threads; i++) {
        ids[i] = i;
        Pthread_create(&t[i], NULL, worker, &ids[i]);
    }

    for (i = 0; i < num_threads; i++) {
        Pthread_join(t[i], NULL);
    }

    printf("parent: end, value %d\n", value);

    sem_destroy(&mutex.lock);
    sem_destroy(&mutex.lobby);
    sem_destroy(&mutex.area0);

    return 0;
}
