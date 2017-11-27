//
// Created by gaochong on 17-11-24.
//

#include "GCIP.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

GCIP::GCIP(void) {
}


GCIP::~GCIP(void) {
}


unsigned long GCIP::IPStrToNaddress(const char *IpStr) {

    unsigned long nip;
    nip = inet_addr(IpStr);
    return nip;
}

char *GCIP::NaddressToIPStr(unsigned long Naddress) {

    struct in_addr in;
    in.s_addr = Naddress;
    return inet_ntoa(in);
}


char *GCIP::NextIPaddress(const char *ipAdd) {
    return NaddressToIPStr(ntohl(ntohl(IPStrToNaddress(ipAdd)) + 1));
}

int GCIP::IP2Count(const char *ip1, const char *ip2) {
    return ntohl(IPStrToNaddress(ip2)) - ntohl(IPStrToNaddress(ip1));
}

char *GCIP::IPToMAC(char *IpAddStr) {


}

char *GCIP::IPGetPCName(const char *ipAdd) {

    int server_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in SrvAddr;
    memset(&SrvAddr, 0, sizeof(SrvAddr));
    SrvAddr.sin_addr.s_addr = inet_addr(ipAdd);
    SrvAddr.sin_port = htons(137);
    SrvAddr.sin_family = AF_INET;

    TNetBiosNS nbns;

    InitNetBiosNs(&nbns);

    char *hostname;

    int errorcode = sendto(server_sockfd, (char *) &nbns, sizeof(nbns), 0, (struct sockaddr *) &SrvAddr,
                           sizeof(SrvAddr));

    printf("sendto Error : %d\n", errorcode);

    if (errorcode < 0) return NULL;

    char recvBuf[512];
    //memset(recvBuf,0,sizeof(recvBuf)*sizeof(char));
    bzero(recvBuf, sizeof(recvBuf));
    socklen_t destlen = sizeof(SrvAddr);


    struct timeval tv_out;
    tv_out.tv_sec = 5;//5 秒超时
    tv_out.tv_usec = 0;

    setsockopt(server_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));

    if (errorcode = recvfrom(server_sockfd, recvBuf, sizeof(recvBuf), 0, (struct sockaddr *) &SrvAddr, &destlen) < 1) {

        hostname = NULL;
        printf("recvfrom Error: %d\n", errorcode);

    } else {

        for (int i = 57; i < 57 + 18; i++) {

            if (recvBuf[i] < 0x20)

                recvBuf[i] = 0;
        }

        hostname = &recvBuf[57];
        printf("recvfrom :%s\n", hostname);
    }

    close(server_sockfd);

    return hostname;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GCIP::InitNetBiosNs(TNetBiosNS *nbns) {
    nbns->tID = 0x0000;
    nbns->Flags = 0x1000;
    nbns->Questions = 0x0100;
    nbns->AnswerRRs = 0x0000;
    nbns->AuthorityRRs = 0x0000;
    nbns->AdditionalRRs = 0x0000;
    nbns->Name[0] = 0x20;
    nbns->Name[1] = 0x43;
    nbns->Name[2] = 0x4b;
    for (int i = 3; i < 33; i++)
        nbns->Name[i] = 0x41;
    nbns->Name[33] = 0x00;
    nbns->tType = 0x2100;
    nbns->tClass = 0x0100;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

