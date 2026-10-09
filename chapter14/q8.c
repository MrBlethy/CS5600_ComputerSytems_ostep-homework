#include <stdio.h>
#include <stdlib.h>

int *resize_fib(int size) {

    if (size <= 0) {
        return NULL;
    }

    // Allocate initial memory for 2 integers
    int capacity = 2;
    int *arr_1 = malloc(capacity * sizeof(int));

    if (arr_1 == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    // Initialize Fibonacci sequence
    arr_1[0] = 1;

    if (size > 1) {
        arr_1[1] = 1;
    }

    // Generate Fibonacci numbers
    for (int i = 2; i < size; i++) {

        // Double capacity when the array is full
        if (i >= capacity) {

            int new_capacity = capacity * 2;

            int *arr_2 = realloc(arr_1, new_capacity * sizeof(int));

            if (arr_2 == NULL) {
                printf("Memory reallocation failed\n");
                free(arr_1);
                return NULL;
            }

            arr_1 = arr_2;
            capacity = new_capacity;
        }

        // Calculate the next Fibonacci number
        arr_1[i] = arr_1[i - 1] + arr_1[i - 2];
    }

    return arr_1;
}

int main(void) {

    int size = 20;

    int *fib = resize_fib(size);

    if (fib == NULL) {
        return 1;
    }

    // Print the Fibonacci sequence
    for (int i = 0; i < size; i++) {
        printf("%d ", fib[i]);
    }

    printf("\n");

    // Release allocated memory
    free(fib);

    return 0;
}
