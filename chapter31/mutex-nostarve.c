#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include "common_threads.h"

typedef struct __ns_mutex_t {
    sem_t lock;          // Protects the waiting counter. passion this stage allows thread to request acess to the lobby 
    sem_t lobby;         // Gate for joining the group of waiting thread 
    sem_t area0;         // Gate to the critical section
    int waiters_count;   // Threads waiting to join the group waiting to access the lobby 
    int now_serving;     // Group members still waiting to enter critical area
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
    m->waiters_count = 0;
    m->now_serving = 0;

    sem_init(&m->lock, 0, 1); // set open default
    sem_init(&m->lobby, 0, 1);// set open by default
    sem_init(&m->area0, 0, 0); // intentionally closed by default requires permission 
}

void ns_mutex_acquire(ns_mutex_t *m) {
    // Register my arrival with front desk
    sem_wait(&m->lock);
    m->waiters_count++;
    sem_post(&m->lock);

    // enter the lobby and join the group waiting to be granted acess to the area 0 
    sem_wait(&m->lobby);
    m->now_serving++;
    printf("thread waiting to access area 0\n");

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
        printf("Thread %d requesting access to Lobby\n",id );
        ns_mutex_acquire(&mutex);
        
        // Critical section.
        printf("Access Granted\n");
        printf("thread %d inside Area 0\n", id);
        value++;
        printf("value %d\n", value);
        printf("thread %d leaving Area 0\n", id);

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
