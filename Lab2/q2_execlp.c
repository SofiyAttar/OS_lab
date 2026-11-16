#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{   //execlp searches for ls in the path.
    execlp("ls", "ls", "-l", (char *)NULL);
    perror("execlp failed");
    return 1;
}
