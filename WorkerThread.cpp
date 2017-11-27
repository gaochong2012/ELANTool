//
// Created by gaochong on 17-11-27.
//

#include "WorkerThread.h"
#include "QDebug"
#include "QCTools.h"
#include "TableRecord.h"


WorkerThread::WorkerThread( MainWindow *mainUI , TLanNet *tLanNet, QObject *parent) : QThread(parent) {

    this->mainUI = mainUI;
    this->tLanNet = tLanNet;
    //find  = new GCSQLite("MACDB2.db");
}

WorkerThread::~WorkerThread() {

}


void WorkerThread::run() {

    testarp = new GCArp;
    testip  = new GCIP;


    LanTableRecord *ltRecord  = new LanTableRecord;

    const char *mac = testarp->GetRemoteIPMAC(  this->tLanNet->Local ,this->tLanNet->LocalIP ,this->tLanNet->DIP );
    //qDebug() << "mac: " << mac;

    ltRecord->IP = QCTools::PCharToQString( this->tLanNet->DIP );

    if (  mac != NULL ){

        QString DesMAC = QCTools::PCharToQString(  mac );
        ltRecord->MAC = DesMAC;
        ltRecord->Organization = "";
        ltRecord->HOSTNAME = QCTools::PCharToQString( testip->IPGetPCName( this->tLanNet->DIP ) );
        /** 向主线程发送消息 ( 探测结果　)*/
        emit notify( ltRecord );
    }

    emit finished();

    QThread::run();

}
