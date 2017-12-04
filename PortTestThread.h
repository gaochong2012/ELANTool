//
// Created by gaochong on 17-12-1.
//

#ifndef ELANTOOL_PORTTESTTHREAD_H
#define ELANTOOL_PORTTESTTHREAD_H


#include <QtCore/QThread>
#include "SStruct.h"

class PortTestThread : public QThread{

Q_OBJECT

    private:
        QList<PortData> selport;
        char  ip[20];

    public:

        PortTestThread( char* ip ,  QObject *parent = nullptr);
        PortTestThread( char* ip , QList<PortData> selport , QObject *parent = nullptr);

        virtual ~PortTestThread();

    protected:
        void run() override;

signals:
    void OnNotifyPortTest( int port ,int type  );



};


#endif //ELANTOOL_PORTTESTTHREAD_H
