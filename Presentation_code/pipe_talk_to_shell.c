#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#define MESSAGE_SIZE 20

int fds[2];
char pipe_buffer[MESSAGE_SIZE];

int main()
{
    char mes1[MESSAGE_SIZE] = "Hello World";
    FILE *file;
    int status = 0;

    file = popen("ls -l", "w");

    status = pclose(file);
    if (status == -1)
    {
        exit(1);
    }

    return 0;
}