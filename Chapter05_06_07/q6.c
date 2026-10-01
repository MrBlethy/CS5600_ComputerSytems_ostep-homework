#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main( int argc, char *argv[]){
   int rc = fork();

   if (rc < 0 ){
      fprintf(stderr, "fork failed\n");
      exit(1);
   } else if (rc == 0){
      printf("child process: (PID: %d)\n", (int) getpid());
   } else {
     int wc = waitpid(rc, NULL, 0);
       printf("parent with PID:%d, waits for child PID : %d, waitpid() returned: %d\n", (int) getpid(), rc, wc);
   }
   return 0;
}
