#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    //open a file before calling fork 
    int fd = open ("q2.out", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    //open dailed check
    if (fd < 0) {
    	fprintf(stderr, "open failed\n");
	exit(1);
    }
    //fork failed check
    int rc = fork();
    if(rc <0){
       fprintf(stderr, "fork failed");
       exit(1);
    } else if (rc == 0){
	    //child process write to the file descriptor
    char *childmsg = "Child says HELLO World!\n";
    write(fd, childmsg, strlen(childmsg));
    } else {
	    //child process write to the file descriptor
    char *pmsg = "Parent here says What's UP!\n";
    write(fd, pmsg, strlen(pmsg));
    }

    close(fd);
    return 0;
}
