#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main()
{
    // Get key for the shared memory
    int key = ftok("abc", 65); // name of the shared memory, project num

    // Create shared memory for file
    int shmid = shmget(key, 1024, 0777 | IPC_CREAT);

    // Map virtual memory to the shared memory
    char *addr = (char *)shmat(shmid, NULL, 0);

    printf("Data from shared memory: %s\n", addr);

    // Unmap virtual memory
    shmdt(addr);

    // Free shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
