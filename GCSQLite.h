//
// Created by gaochong on 17-11-27.
//

#ifndef ELANTOOL_GCSQLITE_H
#define ELANTOOL_GCSQLITE_H

#include <QString>
#include <QList>
#include <QtSql/QSqlDatabase>
#include "SStruct.h"


class GCSQLite {


public:

    GCSQLite( QString dbPath  );


    QString FindMADI(QString MAC);
    QList<PortData> ReadALLPort();

    virtual ~GCSQLite();

private:
    QSqlDatabase m_dbconn;

    void initDBTable();


};


#endif //ELANTOOL_GCSQLITE_H
