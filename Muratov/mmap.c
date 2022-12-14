#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

int main() {

    int fd = open("mmap.txt", 0666 | O_RDWR);
    int buf_size = 100;
    
    char* map = (char* )mmap(NULL, buf_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    printf("before: %s\n", map);

    for (int i = 0; i < 2; i++) {

        map[i] = 'i';
    }

    printf("after: %s\n", map);

    munmap(NULL, buf_size);
    close(fd);

    return 0;
}