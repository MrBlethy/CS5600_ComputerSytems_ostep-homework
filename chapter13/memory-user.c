#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if(argc <2){
        printf("usage: %s <MB>\n", argv[0]);
	return 1;
    }
    size_t MB = atoi(argv[1]); //convert "250" to 250
    size_t byte = MB * 1024 *1024; //convert MB to bytes
    size_t numbers_of_entries = byte/sizeof(int); // how many entries of would i need to cover the array of specified size 
    int *array = malloc(sizeof(int) * numbers_of_entries );
    if (array == NULL){
    	printf("malloc failed to allocated memory");
	return 1;  //allocation faillure check 
    }
    printf("Running with %d MB of memory.\nNote that I will run forever.\nCTRL C to force quit", MB);

    while(1){
    	for(size_t i = 0; i < numbers_of_entries; i++){   //loop forever
	    array[i] = 1;
	}
    }			
    free(array);
    return 0; 
}
