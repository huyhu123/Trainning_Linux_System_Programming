#include "signal_handle.h"

int main()
{
    // Ignore SIGINT signal
    signal(SIGINT, SIG_IGN);

    printf("Doing some work ...\n");
    while (1)
    {
        
    }

    return 0;
}