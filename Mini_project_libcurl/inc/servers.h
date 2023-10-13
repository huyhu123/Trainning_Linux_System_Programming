#ifndef _SERVERS_H_
#define _SERVERS_H_

#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

// Speedtest by Ookla API URL
#define SPEEDTEST_API_URL "https://www.speedtest.net/api/js/servers"

#define STRING_LENGTH 200
#define FILE_LENGTH 10000

#define TIMEOUT_THRESHOLD 1
#define MAX_SERVER 100

typedef struct
{
    char url[STRING_LENGTH];
    char lat[STRING_LENGTH];
    char lon[STRING_LENGTH];
    int distance;
    char name[STRING_LENGTH];
    char country[STRING_LENGTH];
    char cc[STRING_LENGTH];
    char sponsor[STRING_LENGTH];
    char id[STRING_LENGTH];
    int preferred;
    int https_functional;
    char host[STRING_LENGTH];
    int force_ping_select;
    float latency;
} server_t;

void find_best_server();

int parse_server_data(bool https);

void format_url(int server_num);

void get_servers_latency(int server_num);

void sort_servers_by_latency(int server_num);

void print_servers(int server_num);

size_t discard_response(void *ptr, size_t size, size_t nmemb, void *data);

char *get_server_url(int index);

void set_max_server(int max_server);

#endif