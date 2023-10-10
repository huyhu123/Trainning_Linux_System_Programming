#include "speed_test.h"
#include "input.h"

int main(int argc, char* argv[])
{
    int count = 0;
    int value = 0;

    // Default Settings
    int thread_num = 3;
    bool auto_pick_server = true;
    int protocol = 2; // 1: http, 2: https

    // Check input
    while (count < argc)
    {
        if (strcmp(argv[count], "--help") == 0)
        {
            printf("Usage: \n");
            printf("\t Choose number of thread (Default 3): -t <Number of thread> \n");
            printf("\t Choose protocol (Default https): -p <Protocol (http/https)> \n");
            printf("\t Choose server manually: -m \n");
            return 0;
        }

        // Number of thread
        if (strcmp(argv[count], "-t") == 0)
        {
            count++;
            //printf("count: %i, argc: %i\n", count, argc);
            if (count >= argc)
            {   
                printf("Wrong input format (For help: --help)\n");
                return 0;
            }
            // Check if argv[count] is interger
            if (!is_integer(argv[count], &thread_num, 0, 100))
            {
                printf("Wrong input format (For help: --help)\n");
                return 0;
            }
        }

        // Protocol
        if (strcmp(argv[count], "-p") == 0)
        {
            count++;
            //printf("count: %i, argc: %i\n", count, argc);
            if (count >= argc)
            {   
                printf("Wrong input format (For help: --help)\n");
                return 0;
            }
            // Check if argv[count] is interger
            if (strcmp(argv[count], "http") == 0)
            {
                protocol = 1;
            }
            else if (strcmp(argv[count], "https") == 0)
            {
                protocol = 2;
            }
            else
            {
                printf("Wrong input format (For help: --help)\n");
                return 0;
            }
        }

        // Auto pick best server
        if (strcmp(argv[count], "-m") == 0)
        {
            auto_pick_server = false;
        }

        count++;
    }

    printf("Thread num: %i\n", thread_num);
    printf("Protocol: %i\n", protocol);
    printf("Auto pick server: %i\n", auto_pick_server);

    run(thread_num, protocol, auto_pick_server);
    return 0;
}