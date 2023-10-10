#ifndef _SPEED_TEST_UTILS_H_
#define _SPEED_TEST_UTILS_H_

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

#define SPEEDTEST_DOMAIN_NAME "www.speedtest.net"
#define CONFIG_REQUEST_URL "speedtest-config.php"
#define FILE_DIRECTORY_PATH "/tmp/"

#define SPEEDTEST_SERVERS_DOMAIN_NAME "c.speedtest.net"
#define SERVERS_LOCATION_REQUEST_URL "speedtest-servers-static.php?"
#define TIME_OUT 5

int get_ipv4_addr(char *domain_name, struct sockaddr_in *servinfo);

int get_ipv4_addr_https(char *domain_name, struct sockaddr_in *servinfo);

int get_http_file(struct sockaddr_in *serv, char *domain_name, char *request_url, char *filename);

int get_https_file(struct sockaddr_in *serv, char *domain_name, char *request_url, char *filename);

double calcDistance(double lat1, double lon1, double lat2, double lon2);

#endif