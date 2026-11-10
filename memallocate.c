#include <stdio.h>
#include <stdlib.h>

//leetcode problem 254
//
typedef struct {
    int* memory;
    int size;
} Allocator;

// Initialize the allocator with n memory units (0 means free)
Allocator* allocatorCreate(int n) {
    Allocator* obj = (Allocator*)malloc(sizeof(Allocator));
    obj->size = n;
    obj->memory = (int*)calloc(n, sizeof(int)); 
    return obj;
}

// Allocate contiguous blocks using a First-Fit approach
int allocatorAllocate(Allocator* obj, int size, int mID) {
    int count = 0;
    for (int i = 0; i < obj->size; i++) {
        if (obj->memory[i] != 0) {
            count = 0; // Reset if block is occupied
        } else {
            count++;
        }
        
        // Found a big enough free block
        if (count == size) {
            int startIdx = i - size + 1;
            for (int j = startIdx; j <= i; j++) {
                obj->memory[j] = mID;
            }
            return startIdx;
        }
    }
    return -1; // Not enough contiguous space
}

// Free all memory assigned to a specific mID
int allocatorFreeMemory(Allocator* obj, int mID) {
    int freedCount = 0;
    for (int i = 0; i < obj->size; i++) {
        if (obj->memory[i] == mID) {
            obj->memory[i] = 0;
            freedCount++;
        }
    }
    return freedCount;
}

// Clean up allocated memory struct
void allocatorFree(Allocator* obj) {
    free(obj->memory);
    free(obj);
}
