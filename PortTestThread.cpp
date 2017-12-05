//
// Created by gaochong on 17-12-1.
//

#include "PortTestThread.h"
#include "GCSocketPortTest.h"


PortTestThread::PortTestThread( char* ip ,QObject *parent) : QThread(parent) {

    strcpy( this->ip ,ip );

}

PortTestThread::PortTestThread(char *ip, QList<PortData> selport, QObject *parent) {

    strcpy( this->ip ,ip );
    this->selport =  selport;

}


PortTestThread::~PortTestThread() {

}


void PortTestThread::run() {


    foreach(  PortData  selportData , this->selport  ){

         GCSocketPortTest *test = new GCSocketPortTest;

         int port =   selportData.port.toInt( NULL,10 );

         if ( selportData.Protocol == "tcp" ){

             if( test->TestTCPConnectPort(  ip ,port )  == 0 ){

                 //printf( "########### %s:%d ok #################\n", ip,port  );
                 emit OnNotifyPortTest( port ,0 );
             }else{
                 //printf( "########### %s:%d error ################\n", ip,port  );
                 emit OnNotifyPortTest(  port ,-10 );
             }

         }else{

             if( test->TestUDPSendPort(  ip ,port )  == 0 ){

                 emit OnNotifyPortTest( port ,0 );
             }else{

                 emit OnNotifyPortTest(  port ,-10 );
             }

         }

    }

/**
    //int port[10] = { 21,22,137,138,80,443,8080,3389,9000,3306  };
    for( int i = 0 ; i < 10 ; i++){

        GCSocketPortTest *test = new GCSocketPortTest;

        if( test->TestTCPConnectPort(  ip ,port[i] )  == 0 ){

           printf( "########### %s:%d ok #################\n", ip,port[i]  );

           int portN = port[i];
           emit OnNotifyPortTest( portN ,0 );

       }else{

           printf( "########### %s:%d error ################\n", ip,port[i]  );

           emit OnNotifyPortTest(  port[i] ,-10 );
       }

    }
**/

   // QThread::run();
}


