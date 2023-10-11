#ifndef _SPEED_TEST_SERVER_H_
#define _SPEED_TEST_SERVER_H_

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

#define NEAREST_SERVERS_NUM 5

typedef struct client_data
{
    char ipAddr[128];
    double latitude;
    double longitude;
    char isp[128];
} client_data_t;

typedef struct server_data
{
    char url[128];
    double latitude;
    double longitude;
    char name[128];
    char country[128];
    double distance;
    int latency;
    char domain_name[128];
    struct sockaddr_in servinfo;
} server_data_t;

int get_ip_address_position(char *fileName, client_data_t *client_data);

int get_best_server(server_data_t *nearest_servers);

int compare_latency(const void *a, const void *b);

int get_nearest_server(double lat_c, double lon_c, server_data_t *nearest_servers);

int check_server(server_data_t server_data);

int check_server_https(server_data_t server_data);

int get_nearest_server_https(double lat_c, double lon_c, server_data_t *nearest_servers);

int get_best_server_https(server_data_t *nearest_servers);

#endif