#include "signal_synchronize.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

bool file_available = true;
int counter = 0;
FILE *output_file;

void signal_handler(int signum)
{

}

void process_B()
{
    signal(SIGUSR1, signal_handler);
    while (1)
    {
        pause();
        
        fseek(output_file, 0, SEEK_SET);
        fscanf(output_file, "%d", &counter);
        counter++;
        fprintf(output_file, "Process B: %d\n", counter);
        fflush(output_file);

        kill(getppid(), SIGUSR1);
        
    }
}

void process_A()
{
    pid_t pid_B;
    output_file = fopen("output.txt", "a");
    if (output_file == NULL)
    {
        perror("Error opening output file");
        exit(1);
    }
    pid_B = fork();
    if (pid_B < 0)
    {
        perror("Error forking process B");
        exit(1);
    }
    else if (pid_B == 0)
    {
        // Child process B
        process_B();
    }
    else
    {
        // Parent process A
        signal(SIGUSR1, signal_handler);
        while (1)
        {
            sleep(1);
            fseek(output_file, 0, SEEK_SET);
            fscanf(output_file, "%d", &counter);
            counter++;
            fprintf(output_file, "Process A: %d\n", counter);
            fflush(output_file);

            kill(pid_B, SIGUSR1);
            pause();
        }
    }
}

int main()
{
    process_A();
    return 0;
}