
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define BUCKETS 100
#define OPERATIONS 10000

// Each bucket contains a linked list of nodes
typedef struct node {
    int key;
    struct node *next;
} node_t;

// Hash table protected by one global mutex
typedef struct {
    node_t *buckets[BUCKETS];
    pthread_mutex_t lock;
} hash_t;

hash_t table;

// Initialize the hash table
void init() {
    for (int i = 0; i < BUCKETS; i++)
        table.buckets[i] = NULL;

    pthread_mutex_init(&table.lock, NULL);
}

// Insert a key into the hash table
void insert(int key) {
    int index = key % BUCKETS;

    pthread_mutex_lock(&table.lock);

    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("malloc");
        exit(1);
    }

    new_node->key = key;
    new_node->next = table.buckets[index];
    table.buckets[index] = new_node;

    pthread_mutex_unlock(&table.lock);
}

// Each worker inserts 10,000 unique keys
void *worker(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < OPERATIONS; i++)
        insert(id * OPERATIONS + i);

    return NULL;
}

// Free all nodes and destroy the mutex
void destroy() {
    for (int i = 0; i < BUCKETS; i++) {
        node_t *current = table.buckets[i];

        while (current != NULL) {
            node_t *next = current->next;
            free(current);
            current = next;
        }
    }

    pthread_mutex_destroy(&table.lock);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <threads>\n", argv[0]);
        return 1;
    }

    int nums_threads = atoi(argv[1]);

    if (nums_threads <= 0)
        return 1;

    pthread_t threads[nums_threads];
    int ids[nums_threads];

    init();

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Create worker threads
    for (int i = 0; i < nums_threads; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    // Wait for all workers
    for (int i = 0; i < nums_threads; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&end, NULL);

    double elapsed =
        (end.tv_sec - start.tv_sec) +
        (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Threads: %d\n", nums_threads);
    printf("Total insertions: %d\n", nums_threads * OPERATIONS);
    printf("Elapsed time: %.6f seconds\n", elapsed);

    destroy();
    return 0;
}

