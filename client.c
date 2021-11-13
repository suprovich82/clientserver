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

#define NUM_CMD 3

int buff_port = 0;
char address[20];
int C_port;
char message[1024];
int sockfd = 0;

struct _Command{
    char name_command[10];
    void ( *P_of_Func ) ( void );
};


void help( void ){
    printf( "Commands available:\n"
            " - help\n""        This help massage\n"
            " - exit\n""        Quit the session\n "
            "- connect  <String>\n""        Connect to server\n"
            " - send  <String>\n""        Send message\n" );
}

void Connect( void ){
    printf( "Please enter address: \n" );
    scanf("%s", address);

    printf( "Please enter port: \n" );
    scanf( "%d", &C_port );

    struct sockaddr_in serv_addr; 

    sockfd = socket( AF_INET, SOCK_STREAM, 0 );

    memset( &serv_addr, '0', sizeof( serv_addr ) ); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( C_port ); 

    inet_pton( AF_INET, address, &serv_addr.sin_addr ); // creates a structure with a network address

    int res = connect( sockfd, ( struct sockaddr * )&serv_addr, sizeof( serv_addr ) );
    
    if ( res != 0 ){
        printf("Error connect \n");
    }
}

void Send( void ){
    printf( "Please enter message: \n" );
    scanf( "%s", message );

    send( sockfd , message , strlen(message) , 0 );

}

void menu( void ){

    struct _Command help_St;
    struct _Command Connect_St;
    struct _Command Send_St;

    struct _Command *buff;
    int exit_flag = 0;

    struct _Command * array_of_pointer[] = { &help_St, &Connect_St, &Send_St };

    strcpy( help_St.name_command, "help" );
    help_St.P_of_Func = help;

    strcpy( Connect_St.name_command, "connect" );
    Connect_St.P_of_Func = Connect;

    strcpy( Send_St.name_command, "send" );
    Send_St.P_of_Func = Send;
 
    char string[100];
    
    while( 0 == exit_flag ){
        printf( "Client> " );    
        scanf( "%s", string );

        if ( strcmp( string, "exit" ) == 0 ){
            exit_flag = 1;
            break;
        }

        for( int i = 0; i < NUM_CMD; i++ ) {   
            buff = array_of_pointer[i];
            
            if ( strcmp( string, buff->name_command ) == 0 ){
                buff->P_of_Func();
            } 
        }
    }
}

int main( int argc, char* argv[] )
{
    printf("Started\n");

     if ( 1 == argc ) {
        menu();
    }
 
    return 0;
}



// int sockfd = 0, n = 0;
//     char recvBuff[1024];
//     struct sockaddr_in serv_addr; 

//     char *message = "Hello from client";
    
//     char NTP_serv_addr[] =  "127.0.0.1"; -address

//     printf("%s\n", NTP_serv_addr);

//     memset(recvBuff, '0',sizeof(recvBuff));
//     sockfd = socket(AF_INET, SOCK_DGRAM, 0);

//     memset(&serv_addr, '0', sizeof(serv_addr)); 

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT); 

//     inet_pton(AF_INET, NTP_serv_addr, &serv_addr.sin_addr); // creates a structure with a network address

//     connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

//     while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
//     {
//         recvBuff[n] = 0;
//         fputs(recvBuff, stdout);
//     } 

//     if(n < 0)
//     {
//         printf("\n Read error \n");
//     } 

//     send(sockfd , message , strlen(message) , 0 );
//     printf("Hello message sent\n");

//     int valread = read( sockfd , recvBuff, 1024);
//     printf("%s\n",recvBuff );