#include "speed_test.h"
#include "input.h"

int main(int argc, char* argv[])
{
    int count = 0;

    // Default Settings
    int thread_num = 3;
    bool auto_pick_server = true;
    int protocol = 1; // 1: http, 2: https

    // Check input
    while (count < argc)
    {
        if (strcmp(argv[count], "-thread") == 0)
        {
            count++;
            //printf("count: %i, argc: %i\n", count, argc);
            if (count >= argc)
            {   
                printf("Wrong input format (For help: --help)\n");
                return 0;
            }
            // Check if argv[count] is interger
            // Todo
        }

        printf("%s\n", argv[count]);
        count++;
    }

    run(thread_num, protocol, auto_pick_server);
    return 0;
}