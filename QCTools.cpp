//
// Created by gaochong on 17-11-24.
//

#include "QCTools.h"
#include <QDebug>


/** QString转char */
const char *QCTools::QStringTOpchar(QString str) {

    QByteArray ba = str.toLatin1();
    char *pchar = ba.data();

    //qDebug()<< pchar;
    return pchar;
}

/** char *转QString */
QString QCTools::PCharToQString( const char *charz ){

    QString str = QString( QLatin1String( charz ) );

    return str;
}

std::string QCTools::QStringTOstring(QString str) {

    std::string sStr;
    sStr = str.toStdString();

    return  sStr;
}

QString QCTools::stringToQString(std::string sStr) {

    QString Str = QString::fromStdString( sStr );

    return Str;
}
