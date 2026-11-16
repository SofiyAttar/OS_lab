#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{   pid_t pid;
    pid = fork();
    if(pid<0){
        printf("fork failed");
        return 1;
    }
    if(pid==0)
    {   // Child process
        printf("I am child, PID = %d\n", getpid());
        exit(0);
    }
    else
    {   // Parent waits for child
        wait(NULL);
        printf("I am parent, PID = %d\n", getpid());
    }
    return 0;
}
