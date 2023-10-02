#include "mutex_synchronize.h"

int shared_resource = 0;
pthread_mutex_t mutex;

void init_mutex()
{
    // Initialize mutex lock
    pthread_mutex_init(&mutex, NULL);
}

void destroy_mutex()
{
    // Destroy mutex lock
    pthread_mutex_destroy(&mutex);
}

void *thread_function(void *arg)
{
    int thread_id = *(int *)arg;
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        // Lock shared resourse
        pthread_mutex_lock(&mutex);

        // Critical section
        shared_resource++;
        printf("Thread %d incremented shared resource to %d\n", thread_id, shared_resource);
        sleep(1);

        // Unlock shared resourse
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}