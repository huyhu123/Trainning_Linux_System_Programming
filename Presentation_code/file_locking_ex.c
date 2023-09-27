#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/file.h>

int main() {
    int fd = open("file.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Acquire shared lock
    if (flock(fd, LOCK_EX) == -1) {
        perror("flock");
        exit(1);
    }

    printf("Acquired exclusive lock on file\n");

    // Hold lock for 10 seconds
    sleep(10);

    // Release lock
    if (flock(fd, LOCK_UN) == -1) {
        perror("flock");
        exit(1);
    }

    printf("Released lock on file\n");

    close(fd);
    return 0;
}