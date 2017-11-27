//
// Created by gaochong on 17-11-27.
//

#ifndef ELANTOOL_GCSQLITE_H
#define ELANTOOL_GCSQLITE_H

#include <QString>
#include <QList>
#include <QtSql/QSqlDatabase>





class GCSQLite {


public:

    GCSQLite( QString dbPath  );


    QString FindMADI(QString MAC);

private:
    QSqlDatabase m_dbconn;

    void initDBTable();

};


#endif //ELANTOOL_GCSQLITE_H
