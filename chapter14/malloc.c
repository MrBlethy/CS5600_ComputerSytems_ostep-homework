#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[]){

	if (argc < 2){
	printf("usage %s\n", argv[0]);
	return 1;
	
}

int arr_size = atoi(argv[1]);
int *arr = malloc(arr_size * sizeof(int)); 
if( arr == NULL){
    printf("memory allocation failed");
    return 1;
}
for(int i = 0; i < arr_size; i++){
    arr[i] = i; 
}
return 0; 
}
