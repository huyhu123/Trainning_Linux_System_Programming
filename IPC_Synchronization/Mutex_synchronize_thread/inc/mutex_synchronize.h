#ifndef _MUTEX_SYNCHRONIZE_H_
#define _MUTEX_SYNCHRONIZE_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

void *thread_function(void *arg);

void init_mutex();

void destroy_mutex();

#endif
