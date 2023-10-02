#ifndef _SEMAPHORE_MULTITHREAD_H_
#define _SEMAPHORE_MULTITHREAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS      3
#define FILE_NAME_LENGTH 255

void *thread_function(void *arg);

void free_semaphore();

void init_semaphore();

#endif
