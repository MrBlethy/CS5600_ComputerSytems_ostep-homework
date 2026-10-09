#include <stdio.h>
#include <stdlib.h>

int* create_fibonacci(int size) {

    if (size <= 0) {
        return NULL;
    }

    // Allocate enough memory for the entire sequence
    int *arr = malloc(size * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    arr[0] = 1;

    if (size > 1) {
        arr[1] = 1;
    }

    for (int i = 2; i < size; i++) {
        arr[i] = arr[i - 1] + arr[i - 2];
    }

    return arr;
}

int main(void) {

    int size = 20;

    int *fib = create_fibonacci(size);

    if (fib == NULL) {
        return 1;
    }

    for (int i = 0; i < size; i++) {
        printf("%d ", fib[i]);
    }

    printf("\n");

    free(fib);

    return 0;
}
