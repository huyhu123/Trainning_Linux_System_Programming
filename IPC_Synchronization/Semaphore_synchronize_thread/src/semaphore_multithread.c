#include "semaphore_multithread.h"

int common_variable = 0;
sem_t sem1 = {0}; // Semaphore for file output 1
sem_t sem2 = {0}; // Semaphore for file output 2

void write_to_file(char *dir, int num)
{
    FILE *fp = NULL;
    fp = fopen(dir, "a");
    fprintf(fp, "%i\n", num);
    fclose(fp);
}

int random_int(int min, int max)
{
    // Generate a random number in the range [min, max]
    return (rand() % (max - min + 1)) + min;
}

void init_semaphore()
{
    // Initialize semaphore
    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 1);
}

void free_semaphore()
{
    // Free semaphore object
    sem_destroy(&sem1);
    sem_destroy(&sem2);
}

void *thread_function(void *arg)
{
    char filename[FILE_NAME_LENGTH] = {0};
    int rand = 0;
    int val = 0;
    int thread_id = *(int *)arg;

    for (int i = 0; i < 10; i++)
    {
        // Clear string
        memset(filename, 0, str_len(filename));

        // Random output file
        rand = random_int(1, 2);
        sprintf(filename, "output%d.txt", rand);

        switch (rand)
        {
        case 1:
            // Waiting on a Semaphore
            sem_wait(&sem1);

            // Increasment common variable
            common_variable++;

            // Write value to output file and sleep for 1-3s
            printf("Thread %i: write value %i to file %s\n", thread_id, common_variable, filename);
            write_to_file(filename, common_variable);
            sleep(random_int(1, 3));

            // Posting a Semaphore
            sem_post(&sem1);
            break;
        case 2:
            // Waiting on a Semaphore
            sem_wait(&sem2);

            // Increasment common variable
            common_variable++;

            // Write value to output file and sleep for 1-3s
            printf("Thread %i: write value %i to file %s\n", thread_id, common_variable, filename);
            write_to_file(filename, common_variable);
            sleep(random_int(1, 3));

            // Posting a Semaphore
            sem_post(&sem2);
            break;
        default:
            break;
        }
    }
    pthread_exit(NULL);
}

