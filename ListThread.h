//
// Created by gaochong on 17-11-28.
//

#ifndef ELANTOOL_LISTTHREAD_H
#define ELANTOOL_LISTTHREAD_H


#include <QtCore/QThread>
#include "GCArp.h"
#include "GCIP.h"
#include "TableRecord.h"


struct TLanNetList {

    char Local[16];
    //char LocalMAC[20];
    char LocalIP[20];
    char SIP[20];
    char DIP[20];

};


class ListThread : public QThread{

Q_OBJECT

private:
    //MainWindow* mainUI;
    TLanNetList* tLanNetList;

    //GCArp       *testarp ;
    GCIP        *testip;

public:
    ListThread( TLanNetList* tLanNetList,  QObject *parent = nullptr );

    virtual ~ListThread();

protected:
    void run() override;

signals:
    void notify( LanTableRecord *ltRecord  );
    void notifyNextIP( QString nip  );

};


#endif //ELANTOOL_LISTTHREAD_H
