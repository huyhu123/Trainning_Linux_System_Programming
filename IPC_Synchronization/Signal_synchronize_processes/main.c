#include "signal_synchronize.h"

/*
 Process A forks process B and opens the output file for writing.
 Both processes increment the counter variable and write their process ID and the new value of the counter variable to the output file.
 The signal_handler() function is used to interrupt the sleep() call in each process when a signal is received.
 The signal() function is used to register the signal_handler() function to handle the SIGUSR1 signal.
 Process A sends the SIGUSR1 signal to process B after incrementing the counter variable and writing to the output file, and then waits for a signal from process B before continuing.
 Process B waits for a signal from process A before incrementing the counter variable and writing to the output file, and then sends a signal to process A to indicate that it has finished.
 The pause() function is used to wait for a signal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile int counter = 0;
FILE *output_file;

void signal_handler(int signum) {
    // Do nothing, just interrupt the sleep() call
}

void process_B() {
    signal(SIGUSR1, signal_handler);
    while (1) {
        sleep(1);
        fseek(output_file, 0, SEEK_SET);
        fscanf(output_file, "%d", &counter);
        counter++;
        fprintf(output_file, "Process B: %d\n", counter);
        fflush(output_file);
        kill(getppid(), SIGUSR1);
        pause();
    }
}

void process_A() {
    pid_t pid_B;
    output_file = fopen("output.txt", "a");
    if (output_file == NULL) {
        perror("Error opening output file");
        exit(1);
    }
    pid_B = fork();
    if (pid_B < 0) {
        perror("Error forking process B");
        exit(1);
    } else if (pid_B == 0) {
        // Child process B
        process_B();
    } else {
        // Parent process A
        signal(SIGUSR1, signal_handler);
        while (1) {
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

int main() {
    process_A();
    return 0;
}