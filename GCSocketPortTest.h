//
// Created by gaochong on 17-12-1.
//

#ifndef ELANTOOL_GCSOCKETPORTTEST_H
#define ELANTOOL_GCSOCKETPORTTEST_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class GCSocketPortTest {

public:
    int TestUDPSendPort(char *IP, int port);
    int TestTCPConnectPort(char *IP, int port);


};


#endif //ELANTOOL_GCSOCKETPORTTEST_H
