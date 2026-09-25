#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    
    int rc = fork();

    //fork failed check
       if(rc < 0){
           fprintf(stderr, "fork failed");
           exit(1);
    }  else if (rc == 0){  //child process says hello 
       printf( "Child says : HELLO (pid:%d)\n", (int) getpid());
    } else {
	    //Parent process says Goodbye
	    usleep(50000);//ensure a pause to ensure child process runs first
	    printf("parent says: Goodbye (pid:%d)\n", (int) getpid());
    }

    return 0;
}
