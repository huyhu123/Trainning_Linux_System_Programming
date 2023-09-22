#include "signal_handle.h"

void handle_signal(int sig)
{
    printf("\nReceived signal %d", sig);
}
