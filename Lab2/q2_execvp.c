#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{   //Store the command and its arguments in an array.
    char *args[] = {"ls", "-l", NULL};
    execvp("ls", args);
    perror("execvp failed");
    return 1;
}
