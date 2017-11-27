//
// Created by gaochong on 17-11-27.
//

#ifndef ELANTOOL_WORKERTHREAD_H
#define ELANTOOL_WORKERTHREAD_H

#include <QtCore/QThread>

#include "GCSQLite.h"
#include "GCArp.h"
#include "GCIP.h"


class MainWindow;
#include "MainWindow.h"


struct TLanNet {

    char Local[16];
    //char LocalMAC[20];
    char LocalIP[20];
    char DIP[20];

};


class WorkerThread : public QThread{

    Q_OBJECT

private:
    MainWindow* mainUI;
    TLanNet*    tLanNet;

    GCArp       *testarp ;
    GCIP        *testip;

protected:
    void run() override;

public:
    WorkerThread( MainWindow *mainUI , TLanNet *tLanNet, QObject *parent = NULL);

    virtual ~WorkerThread();

signals:
    void notify( LanTableRecord *ltRecord  );
    void finished();
};


#endif //ELANTOOL_WORKERTHREAD_H
