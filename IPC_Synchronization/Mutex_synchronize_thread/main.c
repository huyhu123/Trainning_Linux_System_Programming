#include "mutex_synchronize.h"

int main()
{
    pthread_t threads[NUM_THREADS] = {0};
    int thread_ids[NUM_THREADS] = {0};
    int i = 0;
    init_mutex();
    for (i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    destroy_mutex();
    return 0;
}