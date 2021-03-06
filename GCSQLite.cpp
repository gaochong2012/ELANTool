//
// Created by gaochong on 17-11-27.
//

#include "GCSQLite.h"
#include "SStruct.h"
#include <QDebug>
#include <QtSql/QSqlQuery>


GCSQLite::GCSQLite(QString dbPath) {


    m_dbconn = QSqlDatabase::addDatabase("QSQLITE");

    m_dbconn.setDatabaseName( dbPath );

    if(!m_dbconn.open()){

        qDebug() << "open " << dbPath <<" failed!";

    }else{

        QSqlQuery query( m_dbconn );
        QString TableName = "oui";
        QString  sql = "select count(*) from sqlite_master where type='table' and name='"+ TableName +"'";;

        if(  query.exec( sql ) ){

            if( query.next() ){

                if ( query.value(0).toInt() != 1){
                    initDBTable();
                }
            }
        }

        query.finish();
    }


}


GCSQLite::~GCSQLite() {

    m_dbconn.close();
}


void GCSQLite::initDBTable(){

    qDebug() << "initDBTable() " ;

    QSqlQuery sql_query( m_dbconn );
    QString create_sql = "create table downlist ( "
                            "id             integer PRIMARY KEY autoincrement,"
                            "mac            varchar(8),"
                            "Organization   varchar(128))";

    if ( ! sql_query.exec( create_sql ) ){

        qDebug() << "Error SQLiteFunt::initDBTable()";
    }
    sql_query.finish();

}

QString GCSQLite::FindMADI( QString MAC ){

    QString Organization = "NOT";
    QSqlQuery query( m_dbconn );

    QString sql = "select * from oui where mac = '"+ MAC +"'";
    qDebug() << sql;
    query.exec( sql );


    if( query.next() ){

        Organization = query.value("Organization").toString();


        qDebug()<< "MAC " << MAC << "Organization " << Organization;
    }

    query.finish();

    return Organization;

}

QList<PortData> GCSQLite::ReadALLPort(){

    QSqlQuery query( m_dbconn );

    QString sql = "select * from port";
    qDebug() << sql;
    query.exec( sql );

    QList<PortData> allportlist;
    while( query.next() ){

        PortData  portData;

        portData.id = query.value("id").toString();
        portData.port = query.value("port").toString();
        portData.Protocol = query.value("Protocol").toString();
        portData.company = query.value("company").toString();

        allportlist.append( portData );
    }

    query.finish();

    return allportlist;

}



















