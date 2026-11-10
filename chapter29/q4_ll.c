
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define LIST_SIZE 1000
#define LOOKUPS 10000

// Each node contains a value, pointer, and mutex
typedef struct node {
    int value;
    struct node *next;
    pthread_mutex_t lock;
} node_t;

// Linked list with one global lock
typedef struct {
    node_t *head;
    pthread_mutex_t lock;
} list_t;

list_t list;
int use_handover = 0;

// Initialize the list
void init() {
    list.head = NULL;
    pthread_mutex_init(&list.lock, NULL);
}

// Insert a node (done before threads start)
void insert(int value) {
    node_t *new_node = malloc(sizeof(node_t));

    if (new_node == NULL) {
        perror("malloc");
        exit(1);
    }

    new_node->value = value;
    pthread_mutex_init(&new_node->lock, NULL);

    new_node->next = list.head;
    list.head = new_node;
}

// Lookup using one global mutex
int lookup_global(int value) {
    pthread_mutex_lock(&list.lock);

    node_t *current = list.head;

    while (current != NULL) {
        if (current->value == value) {
            pthread_mutex_unlock(&list.lock);
            return 1;
        }
        current = current->next;
    }

    pthread_mutex_unlock(&list.lock);
    return 0;
}

// Lookup using hand-over-hand locking
int lookup_handover(int value) {
    node_t *current;

    // Protect access to the head pointer
    pthread_mutex_lock(&list.lock);
    current = list.head;

    if (current != NULL)
        pthread_mutex_lock(&current->lock);

    pthread_mutex_unlock(&list.lock);

    while (current != NULL) {
        if (current->value == value) {
            pthread_mutex_unlock(&current->lock);
            return 1;
        }

        node_t *next = current->next;

        // Lock next node BEFORE unlocking current
        if (next != NULL)
            pthread_mutex_lock(&next->lock);

        pthread_mutex_unlock(&current->lock);
        current = next;
    }

    return 0;
}

// Each worker performs the same number of lookups
void *worker(void *arg) {
    (void)arg;

    for (int i = 0; i < LOOKUPS; i++) {
        // Search for a missing value to traverse the full list
        if (use_handover)
            lookup_handover(-1);
        else
            lookup_global(-1);
    }

    return NULL;
}

// Release list memory and mutexes
void destroy() {
    node_t *current = list.head;

    while (current != NULL) {
        node_t *next = current->next;
        pthread_mutex_destroy(&current->lock);
        free(current);
        current = next;
    }

    pthread_mutex_destroy(&list.lock);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <threads> <mode>\n", argv[0]);
        printf("Mode: 0 = global, 1 = hand-over-hand\n");
        return 1;
    }

    int nums_threads = atoi(argv[1]);
    use_handover = atoi(argv[2]);

    if (nums_threads <= 0 || (use_handover != 0 && use_handover != 1)) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    pthread_t threads[nums_threads];

    init();

    // Populate the list before starting workers
    for (int i = 0; i < LIST_SIZE; i++)
        insert(i);

    struct timeval start, end;

    gettimeofday(&start, NULL);

    // Create worker threads
    for (int i = 0; i < nums_threads; i++)
        pthread_create(&threads[i], NULL, worker, NULL);

    // Wait for all workers
    for (int i = 0; i < nums_threads; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&end, NULL);

    double elapsed =
        (end.tv_sec - start.tv_sec) +
        (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Threads: %d\n", nums_threads);
    printf("Mode: %s\n", use_handover ? "Hand-over-hand" : "Global lock");
    printf("Total lookups: %d\n", nums_threads * LOOKUPS);
    printf("Elapsed time: %.6f seconds\n", elapsed);

    destroy();
    return 0;
}

