#ifndef _SPEED_TEST_DOWNLOAD_UPLOAD_H_
#define _SPEED_TEST_DOWNLOAD_UPLOAD_H_

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/evp.h>

#include "speed_test_server.h"

#define MAX_THREAD_NUMBER 10
#define SPEEDTEST_DURATION 4

#define UL_BUFFER_SIZE 8192
#define UL_BUFFER_TIMES 10240
#define DL_BUFFER_SIZE 8192

typedef struct thread
{
    int thread_index;
    int running;
    pthread_t tid;
    char domain_name[128];
    char request_url[128];
    struct sockaddr_in servinfo;
} thread_t;

int speedtest_download(server_data_t *nearest_server, int thread_num);

void *download_thread(void *arg);

int speedtest_upload(server_data_t *nearest_server, int thread_num);

void *upload_thread(void *arg);

void *calculate_dl_speed_thread();

void *calculate_ul_speed_thread();

void stop_all_thread();

#endif