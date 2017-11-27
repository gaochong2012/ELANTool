//
// Created by gaochong on 17-11-24.
//

#ifndef ELANTOOL_QCTOOLS_H
#define ELANTOOL_QCTOOLS_H
#include <string>
#include <QString>

class QCTools {

public:

    static  const char* QStringTOpchar( QString str );
    static  QString PCharToQString( const char  *charz );
    static  std::string  QStringTOstring( QString str );
    static  QString  stringToQString( std::string sStr );

};


#endif //ELANTOOL_QCTOOLS_H
