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

    struct sockaddr addr3 = {0};
    struct in_addr my_sin_addr;
    my_sin_addr.s_addr = inet_addr("127.0.0.1");
    struct sockaddr_in addr2; 
    addr2.sin_family = AF_INET;
    addr2.sin_port = 1000;
    addr2.sin_addr = my_sin_addr;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (struct sockaddr* )&addr2, sizeof(addr2));

    char buf[10];
    int fromlen = sizeof(addr3);
    //sendto(sockfd, (const void* )msg, 6, 0, (struct sockaddr* )&addr, sizeof(addr));
    recvfrom(sockfd, (void* )buf, 10, 0, (struct sockaddr* )&addr3, &fromlen);
    printf("%s\n", buf);
    recvfrom(sockfd, (void* )buf, 10, 0, (struct sockaddr* )&addr3, &fromlen);
    printf("%s\n", buf);    

    return 0;
}