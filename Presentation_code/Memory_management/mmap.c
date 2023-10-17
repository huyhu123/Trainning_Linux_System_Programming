/*
Note: The VirtualBox virtual filesystem apparently 
doesn't implement mmap with the MAP_SHARED option across the boundary of the hypervisor.


*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int fd;
    char *map;
    struct stat st;

    // Open the file
    fd = open("example.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Get the size of the file
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        exit(1);
    }
    off_t  size = st.st_size;

    // Map the file to memory
    map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Close the file (not needed after mapping)
    close(fd);

    // Access and modify the shared memory
    printf("Contents of the file: %s", map);

    // Modify the contents
    map[0] = 'H';
    map[1] = 'e';
    map[2] = 'l';
    map[3] = 'l';
    map[4] = 'o';

    printf("Modified contents of the file: %s", map);

    // Unmap the file from memory
    if (munmap(map, size) == -1) {
        perror("munmap");
        exit(1);
    }

    return 0;
}