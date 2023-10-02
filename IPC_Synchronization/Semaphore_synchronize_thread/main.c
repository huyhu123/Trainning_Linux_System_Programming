#include "semaphore_multithread.h"

int main()
{
    pthread_t threads[NUM_THREADS] = {0};
    int thread_ids[NUM_THREADS] = {0};

    // Initialize semaphore
    init_semaphore();

    // Create thread
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0)
        {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }
    }

    // Join thread
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Error joining thread");
            exit(EXIT_FAILURE);
        }
    }

    // Free semaphore object
    free_semaphore();

    return 0;
}