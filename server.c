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
#define NUM_CMD 4

int port = 0; // define by setPort function
int buff_port = 0;
int sockfd = 0;
char binary[100];

void setPort( void );

struct _Command{
    char name_command[10];
    void ( *P_of_Func ) ( void );
};


void start( void ){
    char cmd[256];
    char port_str[10];
    FILE *fp;

    if ( port != 0 ){
        buff_port = port;
    } else {
        buff_port = PORT;
    }
    
    sprintf(port_str, "%d", buff_port);

    strcpy(cmd, binary);
    strcat(cmd, " start ");
    strcat(cmd, port_str);
    strcat(cmd, "&");

    fp = popen(cmd, "r");
    pclose(fp);
}

void help( void ){
    printf( "Commands available:\n"
            " - help\n""        This help massage\n"
            " - exit\n""        Quit the session\n "
            "- start\n""        Start server\n"
            " - setPort <int>\n""        Set port for server\n "
            "- stop\n""        Stop server\n" );
}

void start_server( void ){
    
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 
    int backlog = 3;
    int addrlen = sizeof( serv_addr );
    int new_socket;
    int valread;
    char Local_serv_addr[] =  "0.0.0.0";

    if ( port != 0 ){
        buff_port = port;
    } else {
        buff_port = PORT;
    }

    memset( recvBuff, '0', sizeof( recvBuff ) );
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );

    memset( &serv_addr, '0', sizeof( serv_addr ) ); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( buff_port ); 

    inet_pton( AF_INET, Local_serv_addr, &serv_addr.sin_addr );
    bind( sockfd, ( struct sockaddr * )&serv_addr, sizeof( serv_addr ) );   
    listen( sockfd, backlog );

    new_socket = accept( sockfd, ( struct sockaddr * )&serv_addr, ( socklen_t* )&addrlen );
    valread = read( new_socket , recvBuff, 1024 );

    recvBuff[valread] = 0;
    fputs( recvBuff, stdout );

}

void setPort( void ){
    printf( "Enter port>" );
    scanf("%d", &port);    
}

void stop( void ){
    FILE *fp;

    fp = popen("ps -ef | grep 'server *start' | grep -v grep |  cut -d' ' -f'4' | xargs kill", "r");
    pclose(fp);
}

void menu( void ){

    struct _Command help_St;
    struct _Command start_St;
    struct _Command setPort_St;
    struct _Command stop_St;
    struct _Command *buff;
    int exit_flag = 0;

    struct _Command * array_of_pointer[] = { &help_St, &start_St, &setPort_St, &stop_St };

    strcpy( help_St.name_command, "help" );
    help_St.P_of_Func = help;

    strcpy( start_St.name_command, "start" );
    start_St.P_of_Func = start;

    strcpy( setPort_St.name_command, "setPort" );
    setPort_St.P_of_Func = setPort;

    strcpy( stop_St.name_command, "stop" );
    stop_St.P_of_Func = stop;
 
    char string[100];
    
    while( 0 == exit_flag ){
        printf( "Server> " );    
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

int main( int argc, char* argv[] ){
    strcpy(binary, argv[0]);

    if ( 1 == argc ) {
        menu();
    } else if ( 3 == argc ){
        port = atoi(argv[2]);

        if ( strcmp( argv[1], "start" ) == 0 && port > 1024 ){
            start_server();
        }
    }

    return 0;
}

