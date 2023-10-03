#include "signal_synchronize.h"

void signal_handler(int signum)
{
    // Signal to resume pause() function
}

void write_to_file_process(char *dir, int num)
{
    // Open file
    FILE *fp = NULL;
    fp = fopen(dir, "a");
    if (fp == NULL)
    {
        printf("Error openning file to write\n");
        exit(1);
    }

    // Append to file
    fprintf(fp, "Process %i: %i\n", getpid(), num);
    fclose(fp);
}

int get_last_line_number(const char *filename)
{
    FILE *fp = NULL;
    char line[LINE_LENGTH] = {0};
    int last_line_number = 0;

    // Open file
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    // Get last line number
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int line_number;
        if (sscanf(line, "Process %*d: %d", &line_number) == 1)
        {
            last_line_number = line_number;
        }
    }

    fclose(fp);
    return last_line_number;
}

void process_b()
{
    int num_b = 0;

    // Register user signal 1
    signal(SIGUSR1, signal_handler);

    while (1)
    {
        // Pause until signal arrive
        pause();

        // Critical section
        sleep(1);
        num_b = get_last_line_number(OUTPUT_FILE);
        num_b++;
        write_to_file_process(OUTPUT_FILE, num_b);
        printf("Process B (PID: %i): write '%i' to '%s'\n", getpid(), num_b, OUTPUT_FILE);

        // Send signal to process A
        kill(getppid(), SIGUSR1);
    }
}

void process_a()
{
    // Create file output if not exist
    FILE *fp = fp = fopen(OUTPUT_FILE, "a");
    if (fp == NULL)
    {
        printf("Error creating file\n");
        exit(1);
    }
    fclose(fp);
    // Create process B
    pid_t pid_B;
    pid_B = fork();
    if (pid_B < 0)
    {
        perror("Error forking process B");
        exit(1);
    }
    else if (pid_B == 0)
    {
        // Child process B
        process_b();
    }
    else
    {
        // Parent process A
        int num_a = 0;

        // Register user signal 1
        signal(SIGUSR1, signal_handler);

        while (1)
        {
            // Critical section
            sleep(1);
            num_a = get_last_line_number(OUTPUT_FILE);
            num_a++;
            write_to_file_process(OUTPUT_FILE, num_a);
            printf("Process A (PID: %i): write '%i' to '%s'\n", getpid(), num_a, OUTPUT_FILE);

            // Send signal to process B
            kill(pid_B, SIGUSR1);

            // Pause until signal arrive
            pause();
        }
    }
}