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
    bool https = false;

    // Check input
    while (count < argc)
    {
        if (strcmp(argv[count], "--help") == 0)
        {
            printf("Usage: \n");
            printf("\t Choose number of thread (Default 3): -t=<Number of thread> \n");
            printf("\t Choose protocol (Default https): -p=<Protocol (http/https)> \n");
            printf("\t Choose server manually (Default auto): -m \n");
            printf("Example: ./main -t=5 -p=https -m \n");
            return 0;
        }
        else if (strcmp(argv[count], "-m") == 0)
        {
            auto_pick_server = false;
            count++;
            continue;
        }
        else
        {
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
            else if (strcmp(token, "-p") == 0)
            {
                token = strtok(NULL, "=");
                
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
            else
            {
                printf("Wrong input format (For help: --help)\n");
                return 0;
            }
        }

        count++;
    }

    if (argc == 1)
    {
        printf("\nRunning with default options, to change option --help\n\n");
    }

    printf("=================Configuration=================\n");
    printf("Thread num: %i\n", thread_num);
    if (https)
    {
        printf("Protocol: %s\n", "https");
    }
    else
    {
        printf("Protocol: %s\n", "http");
    }
    if (auto_pick_server)
    {
        printf("Auto pick server: True\n");
    }
    else
    {
        printf("Auto pick server: False\n");
    }
    printf("===============================================\n");

    // Run speed test
    run_speed_test(thread_num, https, auto_pick_server);

    return 0;
}