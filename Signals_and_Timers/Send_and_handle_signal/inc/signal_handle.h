#ifndef _SIGNAL_HANDLE_H_
#define _SIGNAL_HANDLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

void handle_signal(int sig);

#endif
