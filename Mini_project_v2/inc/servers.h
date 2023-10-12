#ifndef _SERVERS_H_
#define _SERVERS_H_

#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

// Speedtest by Ookla API URL
#define SPEEDTEST_API_URL "https://www.speedtest.net/api/js/servers"

#define MAX_SERVERS 10

#define TIMEOUT_THRESHOLD 1

typedef struct
{
    char url[100];
    char lat[20];
    char lon[20];
    int distance;
    char name[100];
    char country[100];
    char cc[3];
    char sponsor[100];
    char id[10];
    int preferred;
    int https_functional;
    char host[100];
    int force_ping_select;
    float latency;
} Server;

void find_best_server();

int parse_server_data(bool https);

void format_url(int server_num);

void get_servers_latency(int server_num);

void sort_servers_by_latency(int server_num);

void print_servers(int server_num);

size_t discard_response(void *ptr, size_t size, size_t nmemb, void *data);

char *get_server_url(int index);

#endif