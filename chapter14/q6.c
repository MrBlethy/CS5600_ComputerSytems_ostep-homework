#include<stdio.h>
#include<stdlib.h>

int main (int argc, char* argv[]){

int *data = malloc(sizeof(int) * 100);
if (data == NULL){
   printf("memory allocation ailed");
   return 1;
}
free(data);
printf("%d\n", data[4]);
return 1;
}
