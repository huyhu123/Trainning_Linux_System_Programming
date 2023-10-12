#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "servers.h"
#include "speed_test.h"
#include "input.h"

int main(int argc, char* argv[])
{
    int count = 1;
    int value = 0;
    char *token;

    // Default Settings
    int thread_num = 3;
    bool auto_pick_server = true;
    bool https = true;

    // Check input
    while (count < argc)
    {
        if (argc == 1)
        {
            break;
        }

        if (strcmp(argv[count], "--help") == 0)
        {
            printf("Usage: \n");
            printf("\t Choose number of thread (Default 3): -t=<Number of thread> \n");
            printf("\t Choose protocol (Default https): -p=<Protocol (http/https)> \n");
            printf("\t Choose server manually (Default auto): -m \n");
            return 0;
        }

        // Auto pick best server
        if (strcmp(argv[count], "-m") == 0)
        {
            auto_pick_server = false;
            count++;
            continue;
        }

        token = strtok(argv[count], "=");

        // Number of thread
        if (strcmp(token, "-t") == 0)
        {
            token = strtok(NULL, "=");
            // Check if argv[count] is interger
            if (!is_integer(token, &thread_num, 0, 100))
            {
                printf("Wrong input format (For help: --help)\n");
                return 0;
            }
        }

        // Protocol
        if (strcmp(token, "-p") == 0)
        {
            token = strtok(NULL, "=");
            // Check if argv[count] is interger
            if (strcmp(token, "http") == 0)
            {
                https = false;
            }
            else if (strcmp(token, "https") == 0)
            {
                https = true;
            }
            else
            {
                printf("Wrong input format (For help: --help)\n");
                return 0;
            }
        }

        count++;
    }

    printf("Thread num: %i\n", thread_num);
    printf("Protocol: %i\n", https);
    printf("Auto pick server: %i\n", auto_pick_server);

    run_speed_test(thread_num, https, auto_pick_server);

    return 0;
}