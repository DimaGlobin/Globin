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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    struct in_addr my_sin_addr;
    my_sin_addr.s_addr = inet_addr("127.0.0.1");
    struct sockaddr_in addr; 
    addr.sin_family = AF_INET;
    addr.sin_port = 1000;
    addr.sin_addr = my_sin_addr;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (struct sockaddr* )&addr, sizeof(addr));

    const char* msg1 = "hello1";
    sendto(sockfd, (const void* )msg1, 7, 0, (struct sockaddr* )&addr, sizeof(addr));

    const char* msg2 = "hello2";
    sendto(sockfd, (const void* )msg2, 7, 0, (struct sockaddr* )&addr, sizeof(addr));

    return 0;
}