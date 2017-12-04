//
// Created by gaochong on 17-11-28.
//

#include "ListThread.h"
#include "LanIPListView.h"
#include "QCTools.h"

ListThread::ListThread( TLanNetList* tLanNetList, int srCount ,int outTime ,QObject *parent ) : QThread(parent) {

    this->tLanNetList = (TLanNetList*)malloc( sizeof( TLanNetList ) );
    memcpy( this->tLanNetList ,tLanNetList ,sizeof( TLanNetList ) );

    this->srCount = srCount;
    this->outTime = outTime;
}


ListThread::~ListThread() {

    //free( tLanNetList );
    //delete tLanNetList;

}

void ListThread::run() {


    testip =  new GCIP;

    /** 地址段总数　
     *  开始 IP -- 结束 IP
     * */
    int count = testip->IP2Count( tLanNetList->SIP , tLanNetList->DIP );

    /** 目标 IP */
    char nip[20];
    memset( nip, 0, 20);
    strcpy( nip, tLanNetList->SIP );

    for (int i = 0; i <= count; i++) {

        LanTableRecord *ltRecord  = new LanTableRecord;
        GCArp *testarp = new GCArp( this->srCount , this->outTime );

        const char *mac = testarp->GetRemoteIPMAC(  tLanNetList->Local ,tLanNetList->LocalIP ,nip );

        ltRecord->IP = QCTools::PCharToQString( nip );

        if (  mac != NULL ){

            QString DesMAC = QCTools::PCharToQString(  mac );
            ltRecord->MAC = DesMAC;
            ltRecord->Organization = "";
            ltRecord->HOSTNAME = QCTools::PCharToQString( testip->IPGetPCName( nip ) );

            /**
             * 向主线程发送消息 ( 探测结果　)*/
            emit notify( ltRecord );
        }

        /**
         * 取下一个目标ip */
        strcpy( nip ,testip->NextIPaddress( nip ) );

        /** 向主线程发送消息 ( 正要探测的IP 　)*/
        emit notifyNextIP( QCTools::PCharToQString( nip )   );

    }


    QThread::run();
}
