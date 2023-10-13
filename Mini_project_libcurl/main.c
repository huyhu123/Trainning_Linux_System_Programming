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
    int server_num = 20;
    int test_time = 3;
    int time_out = 1;
    bool auto_pick_server = true;
    bool https = false;

    // Check input
    while (count < argc)
    {
        if (strcmp(argv[count], "--help") == 0)
        {
            printf("Usage: \n");
            printf("\t Choose number of thread (Default 3, max 100): -t=<Number of thread> \n");
            printf("\t Choose protocol (Default http): -p=<Protocol (http/https)> \n");
            printf("\t Choose server manually (Default auto): -m \n");
            printf("\t Choose number of nearby servers (Default 20, max 100): -s=<number of nearby server> \n");
            printf("\t Choose request timeout (Default 1s, max 100): -ti=<request timeout time (s)> \n");
            printf("\t Choose test time (Default 3s, max 100): -td=<test time (s)> \n");
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
            else if (strcmp(token, "-s") == 0)
            {
                token = strtok(NULL, "=");
                if (!is_integer(token, &server_num, 0, 100))
                {
                    printf("Wrong input format (For help: --help)\n");
                    return 0;
                }
            }
            else if (strcmp(token, "-ti") == 0)
            {
                token = strtok(NULL, "=");
                if (!is_integer(token, &time_out, 0, 100))
                {
                    printf("Wrong input format (For help: --help)\n");
                    return 0;
                }
            }
            else if (strcmp(token, "-td") == 0)
            {
                token = strtok(NULL, "=");
                if (!is_integer(token, &test_time, 0, 100))
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

    // Set configuration
    set_max_server(server_num);
    set_timeout_threshold(time_out);
    set_test_time(test_time);

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
    printf("Server sum: %i\n", server_num);
    printf("Request timeout time: %i\n", time_out);
    printf("Test time: %i\n", test_time);
    printf("===============================================\n");

    // Run speed test
    run_speed_test(thread_num, https, auto_pick_server);

    return 0;
}