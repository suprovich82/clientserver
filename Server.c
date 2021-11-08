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

#define PORT 3347

int main(void)
{
    printf("Started\n");
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 
    int backlog = 2;
    int addrlen = sizeof(serv_addr);
    
    char Local_serv_addr[] =  "0.0.0.0";

    memset(recvBuff, '0',sizeof(recvBuff));
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( PORT ); 

    inet_pton(AF_INET, Local_serv_addr, &serv_addr.sin_addr);


    int res = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    int res2 = listen(sockfd, 3 );

    int new_socket = accept(sockfd, (struct sockaddr *)&serv_addr, (socklen_t*)&addrlen);

    int valread = read( new_socket , recvBuff, 1024);

    recvBuff[valread] = 0;
    fputs(recvBuff, stdout);

    getchar();

    return 0;
}
