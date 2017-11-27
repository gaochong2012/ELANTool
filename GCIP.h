//
// Created by gaochong on 17-11-24.
//

#ifndef ELANTOOL_GCIP_H
#define ELANTOOL_GCIP_H
#include <string>



class GCIP {

public:
    GCIP(void);
    ~GCIP(void);

    char* IPGetPCName( const char *ipAdd );
    char* IPToMAC( char* IpAddStr );
    int   IP2Count( const char* ip1 , const char* ip2 );
    char* NextIPaddress( const char *ipAdd  );
    char* NaddressToIPStr( unsigned long Naddress );
    unsigned long IPStrToNaddress( const char *IpStr );

private:

    typedef struct TNetBiosNS
    {
        unsigned short tID;
        unsigned short Flags;
        unsigned short Questions;
        unsigned short AnswerRRs;
        unsigned short AuthorityRRs;
        unsigned short AdditionalRRs;
        char Name[34];
        unsigned short tType;
        unsigned short tClass;

    }packed;

    void InitNetBiosNs( TNetBiosNS* nbns );


};


#endif //ELANTOOL_GCIP_H
