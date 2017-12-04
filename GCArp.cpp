//
// Created by gaochong on 17-11-24.
//

#include "GCArp.h"

GCArp::GCArp() {

}


GCArp::GCArp( int srCount , int outTime  ) {

    this->srCount =  srCount;
    this->outTime =  outTime;
}

GCArp::~GCArp() {

}

const char *GCArp::GetRemoteIPMAC( const char *nic_name, const char *nic_ip, const char *des_ip ) {

    if ( arp_main(nic_name, nic_ip, des_ip) == 1 ) {

        return this->strDesMAC;

    } else {

        return NULL;
    }

}

/**
// linux下获取LAN里指定IP的网卡MAC
// In: nic_name: 本地网卡名字, nic_ip:本机IP，des_ip:目的IP
// Out: 1表示获取MAC成功，des_ip已被使用, -1表示发生错误，0表示des_ip未使用
 **/
int GCArp::arp_main( const char *nic_name, const char *nic_ip, const char *des_ip ) {

    strcpy(strETH, nic_name); //"eth0";
    strcpy(strSrcIP, nic_ip); //"10.0.1.77";
    strcpy(strDesIP, des_ip); //"10.0.1.69";

    /** 获取指定接口MAC */
    GetLocalMac( strETH );
    if ( 0 == strcmp((const char *) strLocalMAC, (const char *) "") ){

        return -1;
    }

    /** 获取指定IP网卡的MAC */
    GetMacByIP( strSrcIP, strLocalMAC, strDesIP, strETH );

    printf( "strDesMAC = %s\n", strDesMAC );
    if ( 1 == find_ip_flag ){
        return 1;
    } else {
        return 0;
    }

}

/**
// 获取本地某网卡的MAC
// In: strEth
// Out: 若成功，返回MAC字符串，失败，返回""(空串)
 **/
char *GCArp::GetLocalMac(char *strEth) {

    int s;
    struct ifreq buffer;
    char chBuff[MAX_MAC_LEN];
    unsigned int str_len = 0;
    unsigned int offset = 0;

    memset(chBuff, 0x0, sizeof(chBuff));
    s = socket(PF_INET, SOCK_DGRAM, 0);
    if (-1 == s) {
        return "";
    }
    memset(&buffer, 0x00, sizeof(buffer));
    strcpy(buffer.ifr_name, strEth);  // "eth0"
    if (-1 == ioctl(s, SIOCGIFHWADDR, &buffer)) {

        return "";
    }
    close(s);

    offset = 0;
    for (s = 0; s < 6; s++) {
        memset(chBuff, 0x0, sizeof(chBuff));
        sprintf(chBuff, "%.2X", (unsigned char) buffer.ifr_hwaddr.sa_data[s]);
        str_len = strlen(chBuff);
        memcpy(strLocalMAC + offset, chBuff, str_len);
        offset += str_len;

        if (s < 5) {
            memset(chBuff, 0x0, sizeof(chBuff));
            sprintf(chBuff, "%s", COMPART_MAC);
            str_len = strlen(chBuff);
            memcpy(strLocalMAC + offset, chBuff, str_len);
            offset += str_len;

        }
    }

    return strLocalMAC;
}



/**
// 发送ARP包，并接收ARP应答包，取出MAC
// In: strSrcIP:本机IP，strSrcMAC:本机IP的MAC，strDesIP:被请求应答MAC的IP , strNIC:本地接口名
// Out: 若成功，返回MAC，失败返回""(空串)
**/
char *GCArp::GetMacByIP(char *strSrcIP, char *strSrcMAC, char *strDesIP, char *strNIC) {

    int sockfd;     // socket handle

    struct arp_packet arp;  // arp 请求包
    struct arp_packet arpRes; // arp 应用答包
    struct sockaddr sa;   // eth

    char chSrcIP[24];
    char chDesIP[24];
    char chSrcMAC[24];
    char chNIC[8];
    unsigned int str_len = 0;
    unsigned int offset = 0;
    //int i;
    int s;

    find_ip_flag = 0;

    memset(chSrcIP, 0x00, sizeof(chSrcIP));
    memset(chDesIP, 0x00, sizeof(chDesIP));
    memset(chSrcMAC, 0x00, sizeof(chSrcMAC));
    memset(chNIC, 0x00, sizeof(chNIC));

    sprintf(chSrcIP, "%s", strSrcIP);
    sprintf(chDesIP, "%s", strDesIP);
    sprintf(chSrcMAC, "%s", strSrcMAC);
    sprintf(chNIC, "%s", strNIC);

    char *SRC_IP  = chSrcIP;   // 源IP
    char *DES_IP  =  chDesIP;  // 目的IP
    char *LOCAL_HW = chSrcMAC; // eth0的MAC
    char *DEVICE  =  chNIC;    // 本机接口名

    memset(&arp, 0x00, sizeof(arp));
    memset(&arpRes, 0x00, sizeof(arpRes));

    sockfd = socket(AF_INET, SOCK_PACKET, htons(0x0806));
    if ( sockfd < 0) {
        return "";
    }


    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 250;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));


    /** 创建ARP请求包 */
    arp.ethhdr.ether_type = htons(FRAME_TYPE);
    set_hw_addr((char *) arp.ethhdr.ether_dhost, DES_MAC);
    set_hw_addr((char *) arp.ethhdr.ether_shost, LOCAL_HW);
/**
    printf("%x|", arp.ethhdr.ether_type);
    for (i = 0; i < 6; i++) {
        printf("%d_", arp.ethhdr.ether_dhost[i]);
    }
    printf("|");
    for (i = 0; i < 6; i++) {
        printf("%d_", arp.ethhdr.ether_shost[i]);
    }
*/
    /**
    * 初始化ARP包帧数据
    **/
    arp.arphdr.ar_hrd = htons(HARD_TYPE); // 1
    arp.arphdr.ar_pro = htons(PROTO_TYPE); // 0x0800
    arp.arphdr.ar_op = htons(OP_CODE);  // 1
    arp.arphdr.ar_hln = (unsigned char) (6);
    arp.arphdr.ar_pln = (unsigned char) (4);
/**
    printf("%d|%d|%d|%d|%d|\n", arp.arphdr.ar_hrd, arp.arphdr.ar_pro, arp.arphdr.ar_op, arp.arphdr.ar_hln,
                arp.arphdr.ar_pln);
**/

    set_hw_addr((char *) arp.arphdr.__ar_tha, DES_MAC); // 请求IP的MAC
    set_hw_addr((char *) arp.arphdr.__ar_sha, LOCAL_HW); // 发送者的MAC
    set_ip_addr((char *) arp.arphdr.__ar_tip, DES_IP); // 请求MAC的IP
    set_ip_addr((char *) arp.arphdr.__ar_sip, SRC_IP); // 源IP
    bzero(arp.padding, 18); // 填充18个字节
/**
    for (i = 0; i < 6; i++) {
        printf("%d_", arp.arphdr.__ar_sha[i]);
    }
    printf("|");
    for (i = 0; i < 6; i++) {
        printf("%d_", arp.arphdr.__ar_sip[i]);
    }
    printf("|");
    for (i = 0; i < 6; i++) {
        printf("%d_", arp.arphdr.__ar_tha[i]);
    }
    printf("|");

    for (i = 0; i < 6; i++) {
        printf("%d_", arp.arphdr.__ar_tip[i]);
    }
**/

    /** 发送ARP包
     send arp reply packet */
    memset(&sa, 0x00, sizeof(sa));
    strcpy(sa.sa_data, DEVICE);


    int nSendCount = srCount;
    int nRecvByte = 0;

    while ((nSendCount--) > 0) {

        /**
         * 发送ARP请求包
         */
        if ( sendto(sockfd, &arp, sizeof(arp), 0, (struct sockaddr *) &sa, sizeof(sa)) < 0 ) {
            //printf("Error: 发送ARP包失败 [errno = %d]\n", errno);
            return "";
        }

        /**
         * 接收ARP应答包*
         * */
        int nTryCount = srCount;
        int nAddrLen = sizeof(sa);
        do {


            usleep( outTime );
            /**
             * 阻塞 recvfrom
             * */
            nRecvByte = recvfrom(sockfd, &arpRes, sizeof(arpRes), 0, (struct sockaddr *) &sa, (socklen_t *) &nAddrLen);

            /** 若是所请求IP的ARP应答包，退出while */
            if (nRecvByte >= 60 && 2 == ntohs(arpRes.arphdr.ar_op)) {
                char chBuff[MAX_MAC_LEN];
                /** 格式化IP */
                offset = 0;
                for ( s = 0; s < 4; s++ ) {

                    memset(chBuff, 0x00, sizeof(chBuff));
                    sprintf((char *) chBuff, "%d", (unsigned char) arpRes.arphdr.__ar_sip[s]);
                    str_len = strlen(chBuff);

                    memcpy(strTarIP + offset, chBuff, str_len);
                    offset += str_len;

                    if (s < 3) {
                        memset(chBuff, 0x00, sizeof(chBuff));
                        sprintf((char *) chBuff, "%s", ".");
                        str_len = 1;
                        memcpy(strTarIP + offset, chBuff, str_len);
                        offset += str_len;
                    }
                }

                if (!strcmp(strTarIP, strDesIP)) {
                    //printf("\n请求IP[%s] = 应答IP[%s]\n", strDesIP, strTarIP);
                    find_ip_flag = 1; /* find ip */
                    goto analyse_arp_response;
                }
            }

        } while (nTryCount--);

    }

    analyse_arp_response:

    /**
     *  接收超时，或错误
     */
    if (nRecvByte == -1) {
        close(sockfd);
        return "";
    }

    /**
     * 分析ARP应答包 ..
     */
    char chBuff[MAX_MAC_LEN];
    memset(chBuff, 0x00, sizeof(chBuff));

    /** 格式化IP */
    offset = 0;
    for (s = 0; s < 4; s++) {

        memset(chBuff, 0x00, sizeof(chBuff));
        sprintf((char *) chBuff, "%d", ( unsigned char ) arpRes.arphdr.__ar_sip[s]);
        str_len = strlen(chBuff);
        memcpy(strTarIP + offset, chBuff, str_len);
        offset += str_len;
        if (s < 3) {
            memset(chBuff, 0x00, sizeof(chBuff));
            sprintf((char *) chBuff, "%s", ".");
            str_len = strlen(chBuff);
            memcpy(strTarIP + offset, chBuff, str_len);
            offset += str_len;
        }
    }

    /** 格式化MAC */
    memset(chBuff, 0x00, sizeof(chBuff));
    offset = 0;
    for (s = 0; s < 6; s++) {
        memset(chBuff, 0x00, sizeof(chBuff));
        sprintf((char *) chBuff, "%02X", (unsigned char) arpRes.arphdr.__ar_sha[s]);
        str_len = strlen(chBuff);
        memcpy(strTarMAC + offset, chBuff, str_len);
        offset += str_len;
        if (s < 5) {
            memset(chBuff, 0x00, sizeof(chBuff));
            sprintf((char *) chBuff, "%s", COMPART_MAC);
            str_len = strlen(chBuff);
            memcpy(strTarMAC + offset, chBuff, str_len);
            offset += str_len;
        }
    }

    printf("应答IP[%s] 对应的MAC[%s]\n", strTarIP, strTarMAC);
    close(sockfd);

    /** 返回被请求的MAC */
    strcpy( strDesMAC, strTarMAC );
    return strDesMAC;
}


/**
 * 填充MAC
 * **/
void GCArp::set_hw_addr(char buf[], char *str) {

    int i;
    char c, val;
    for (i = 0; i < 6; i++) {
        if (!(c = tolower(*str++)))
            perror("Invalid hardware address"), exit(1);
        if (isdigit(c))
            val = c - '0';
        else if (c >= 'a' && c <= 'f')
            val = c - 'a' + 10;
        else
            perror("Invalid hardware address"), exit(1);
        buf[i] = val << 4;
        if (!(c = tolower(*str++)))
            perror("Invalid hardware address"), exit(1);
        if (isdigit(c))
            val = c - '0';
        else if (c >= 'a' && c <= 'f')
            val = c - 'a' + 10;
        else
            perror("Invalid hardware address"), exit(1);
        buf[i] |= val;
        if (*str == ':')
            str++;
    }
}


/**
 * 填充IP
 */

void GCArp::set_ip_addr(char *buf, char *str) {

    struct in_addr addr;
    memset(&addr, 0x00, sizeof(addr));
    addr.s_addr = inet_addr(str);
    memcpy(buf, &addr, 4);
}
