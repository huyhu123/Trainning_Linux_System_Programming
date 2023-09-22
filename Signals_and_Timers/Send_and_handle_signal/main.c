#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
    printf("\nReceived signal %d\n", sig);
}

int main() {
    // Set up signal handler
    signal(SIGINT, handle_signal);

    // Send SIGALRM signal after 5 seconds
    alarm(5);

    // Wait for signals
    while (1) {
        sleep(1);
    }

    return 0;
}