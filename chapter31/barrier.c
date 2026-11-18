#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common_threads.h"

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

// You likely need two semaphores to do this correctly, and some
// other integers to track things.

typedef struct __barrier_t {
    sem_t access_gate;
    sem_t exit_gate;
    int count;
    int num_threads;
} barrier_t;


// the single barrier we are using for this program
barrier_t b;

void barrier_init(barrier_t *b, int num_threads) {
    sem_init(&b->access_gate, 0, 1); // gate is locked by default permission required 
    sem_init(&b->exit_gate, 0, 0);  // gate is open first come gets the lock 
    b->num_threads = num_threads;
    b->count = 0;
}

void barrier(barrier_t *b) {
    sem_wait(&b->access_gate);  // Take the access lock; wait if another thread is using it. 

    b->count++;                 // The gate keep count of thread who gains ownership of the lock 

    if (b->count == b->num_threads) {
        for (int i = 0; i < b->num_threads; i++){   // This prevent threads from executing the "After print" counting the thread at the wait block.  
            sem_post(&b->exit_gate);
        }
    }

    sem_post(&b->access_gate);  // Each thread release Lock so another can update.

    sem_wait(&b->exit_gate);    // Each thread picks one ticket. if non are available they wait  
}
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t {
    int thread_id;
} tinfo_t;

void *child(void *arg) {
    tinfo_t *t = (tinfo_t *) arg;
    printf("child %d: before\n", t->thread_id);
    barrier(&b);
    printf("child %d: after\n", t->thread_id);
    return NULL;
}


// run with a single argument indicating the number of 
// threads you wish to create (1 or more)
int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);
    
    int i;
    for (i = 0; i < num_threads; i++) {
	t[i].thread_id = i;
	Pthread_create(&p[i], NULL, child, &t[i]);
    }

    for (i = 0; i < num_threads; i++) 
	Pthread_join(p[i], NULL);

    printf("parent: end\n");
    return 0;
}

