#ifndef _SPEED_TEST_H_
#define _SPEED_TEST_H_

#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "servers.h"
#include "input.h"

#define MAX_LENGTH 500
#define MAX_NUM_THREADS 100

typedef struct
{
    const char *url;
    const char *file_path;
    double avg_speed;
    pthread_mutex_t *mutex;
} ThreadData;

double test_upload_speed(const char *url, const char *file_path, int num_thread);

double test_download_speed(const char *url, int num_thread);

void run_speed_test(int num_thread, bool https, bool auto_pick_server);

void set_timeout_threshold(int timeout_threshold);

void set_test_time(int time);

#endif