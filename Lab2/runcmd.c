#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{    pid_t pid;
    //Check if exactly two arguments are provided.
    if (argc != 3)
    {
        printf("Incorrect number of arguments\n");
        return 1;
    }
    //Create a child process.
    pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }
    //Child process.
    if (pid == 0)
    {
        char *args[] = {argv[1], argv[2], NULL};
        execvp(argv[1], args);
        perror("execvp failed");
        exit(1);
    }
    //Parent process.
    else
    {
        //Wait for the child to finish and reap it.
        if (wait(NULL) == -1)
        {
            perror("wait failed");
            return 1;
        }
        printf("Command successfully completed\n");
    }
    return 0;
}
