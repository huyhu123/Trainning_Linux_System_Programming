#include "signal_handle.h"

int main()
{
    // Set up signal handler
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGALRM, handle_signal);

    // Wait for signals
    while (1)
    {
        // Send SIGALRM signal after 5 seconds
        kill(getpid(), SIGALRM);
        sleep(5);
    }

    return 0;
}