#include<stdio.h>
#include<stdlib.h>

int main (int argc, char* argv[]){

int *data = malloc(sizeof(int) * 100);
if(data == NULL){
    printf("Memory allocation failed\n");
    return 1;
}
 data[100] = 0;
return 1;
}
