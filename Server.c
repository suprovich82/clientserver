#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(void)
{
    printf("Started\n");
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 
    int backlog = 2;
    
    char Local_serv_addr[] =  "127.0.0.1"; // Localhost address

    printf("%s", Local_serv_addr);

    memset(recvBuff, '0',sizeof(recvBuff));
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(222); 

    inet_pton(AF_INET, Local_serv_addr, &serv_addr.sin_addr);

    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(sockfd, backlog);

    getchar();

    return 0;
}
