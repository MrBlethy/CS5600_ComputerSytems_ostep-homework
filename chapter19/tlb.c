#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>


int jumper(int pages, int trials){
	if(pages <= 0 || trials <= 0){
		printf("pages and trials must be positive");
			return 1; 
	}

	//define the time structure 
	struct timeval start, end; 

	//default page size 
	int page_size = 4096;
	//each jump represent the number of integers inside a page 
	int jump = page_size/ sizeof(int);
	//allocating memory for the total length pages after all jumps 
	size_t NUMPAGES = pages * jump; 
	
	// memory allocation and error check
	int *arr = malloc(NUMPAGES * sizeof(int));

	if (arr == NULL){
		printf("memory allocation failed");
		return 1; 
	}
	for(size_t i = 0; i < NUMPAGES; i+= jump){
		arr[i] = 0;
	}
	
	gettimeofday(&start, NULL);
	for(int t = 0; t < trials; t++){
		for(size_t i = 0; i < NUMPAGES; i += jump){
			arr[i] += 1;
		}
	}
	gettimeofday(&end, NULL);

	double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) /1000000.0;
	double total_accessed = (double) pages * trials; 
	double average_time_ns = elapsed * 1000000000.0 / total_accessed; //average time capture in nano seconds 
	printf("The average time per access: %.3f ns\n", average_time_ns);
	free(arr);
	return 0;	
}
 int main(int argc, char* argv[]){

	 if(argc !=3 ){
	 	printf("usage:%s <pages> <trials>", argv[0]);
		return 1;
	 }
	 
	 int pages = atoi(argv[1]);
	 int trials = atoi(argv[2]);

	 printf("pages: %d\n", pages);
	 printf("trials: %d\n", trials);
    	
 	int result = jumper(pages, trials);	 
	return result;
 }


