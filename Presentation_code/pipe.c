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
    int child_pid = 0;
    
    if (pipe(fds) < 0)
    {
        exit(1); 
    }

    child_pid = fork();

    if (child_pid > 0)
    {
        // Child process
        for (int i = 0; i < 5; i++)
        {
            write(fds[1], mes1, MESSAGE_SIZE);
            sleep(1);
        }
        close(fds[1]);
    }
    else
    {
        // Parent process
        for (int i = 0; i < 5; i++)
        {
            int num_read = read(fds[0], pipe_buffer, MESSAGE_SIZE);
            if (num_read == -1)
            {
                printf("read failed\n");
                exit(1);
            }
            else if (num_read == 0)
            {
                printf("write pipe closed\n");
            }
            else
            {
                printf("%s\n", pipe_buffer);
            }
            
        }
    }


    return 0;
}