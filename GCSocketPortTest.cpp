//
// Created by gaochong on 17-12-1.
//

#include <cstring>
#include <zconf.h>
#include "GCSocketPortTest.h"



int GCSocketPortTest::TestTCPConnectPort( char* IP ,int port  ){


    //printf( "TestTCPConnectPort: %s:%d\n" ,IP,port  );

    int client_sockfd;
    int ccc = 0;

    struct sockaddr_in remote_addr;
    //char buf[BUFSIZ];
    memset(&remote_addr,0,sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr( IP );
    remote_addr.sin_port = htons( port );


    if( ( client_sockfd = socket( PF_INET,SOCK_STREAM,0 ) ) < 0 ) {
        perror("socket error");
        return -1;
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 250;
    //SO_SNDTIMEO
    setsockopt( client_sockfd , SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    if( connect( client_sockfd,( struct sockaddr * )&remote_addr,sizeof(struct sockaddr) )<0 ) {

        perror( "connect error" );
        ccc = -2;
    }

    //printf("connected to server/n");

    close(client_sockfd);

    return ccc;


}

int GCSocketPortTest::TestUDPSendPort( char* IP ,int port  ){


    int client_sockfd;
    int len = 0;

    struct sockaddr_in remote_addr;
    int sin_size = 0;
    char buf[BUFSIZ];
    memset(&remote_addr,0,sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr( IP );
    remote_addr.sin_port = htons( port );

    if( (client_sockfd = socket(PF_INET,SOCK_DGRAM,0) )<0  ) {
        perror("socket");
        return -1;
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 200; /** 超时*/

    setsockopt( client_sockfd , SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    strcpy( buf,"This is a test message" );

    //sin_size = sizeof( struct sockaddr_in );

    if( ( len = sendto( client_sockfd,buf,strlen(buf),0,(struct sockaddr *)&remote_addr,sizeof( struct sockaddr ) ) ) < 0 ) {
        perror("recvfrom");
        //return -2;
    }

    close(client_sockfd);

    return 0;

}