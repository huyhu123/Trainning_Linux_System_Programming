#include <stdio.h>
#include <signal.h>

void handle_signal(int sig) {
    printf("Received signal %d\n", sig);
}

int main() {
    // Ignore SIGINT signal
    signal(SIGINT, SIG_IGN);

    // Set up signal handler for other signals
    signal(SIGTERM, handle_signal);
    signal(SIGQUIT, handle_signal);

    // Do some work
    while (1) {
        // Do some work here
    }

    return 0;
}