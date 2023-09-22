#include "signal_handle.h"

int main()
{
    // Set up signal handler
    signal(SIGALRM, handle_signal);

    // Wait for signals
    printf("Send SIGALRM (14) signal after 5 seconds\n");
    while (1)
    {
        // Send SIGALRM signal after 5 seconds
        kill(getpid(), SIGALRM);
        sleep(5);
    }

    return 0;
}