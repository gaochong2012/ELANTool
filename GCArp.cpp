//
// Created by gaochong on 17-11-24.
//

#include "GCArp.h"

GCArp::GCArp() {

}

const char *GCArp::GetRemoteIPMAC(const char *nic_name, const char *nic_ip, const char *des_ip) {

    if (arp_main(nic_name, nic_ip, des_ip) != -1) {

        return this->strDesMAC;

    } else {

        return NULL;
    }

}

// linux下获取LAN里指定IP的网卡MAC
// In: nic_name: 本地网卡名字, nic_ip:本机IP，des_ip:目的IP
// Out: 1表示获取MAC成功，des_ip已被使用, -1表示发生错误，0表示des_ip未使用
int GCArp::arp_main(const char *nic_name, const char *nic_ip, const char *des_ip) {

    strcpy(strETH, nic_name); //"eth0";
    strcpy(strSrcIP, nic_ip); //"10.0.1.77";
    strcpy(strDesIP, des_ip); //"10.0.1.69";

    wenxy_debug("Run ......\n");
    wenxy_debug("获取 %s 接口的MAC ......\n", strETH);
    // 获取指定NIC名称的MAC
    /*strLocalMAC = GetLocalMac(strETH);*/
    GetLocalMac(strETH);

#if 0
    wenxy_debug("Note: %s[%s]\n", strETH, strLocalMAC);
#endif

    // 获取指定接口MAC
    if (0 == strcmp((const char *) strLocalMAC, (const char *) "")) {
        wenxy_debug("Error: call strcmp() failed\n");
        wenxy_debug("--------------------------------\n\n");
        return -1;
    } else {
        wenxy_debug("获取接口MAC成功: %s [%s]\n", strETH, strLocalMAC);
    }

    // 获取指定IP网卡的MAC
    /*strDesMAC = GetMacByIP(strSrcIP, strLocalMAC, strDesIP, strETH);*/
    GetMacByIP(strSrcIP, strLocalMAC, strDesIP, strETH);
    wenxy_debug("strDesMAC = %s\n", strDesMAC);

    if (1 == find_ip_flag) {
        printf("Note:  strDesIP: %s,  strDesMAC: %s\n", strDesIP, strDesMAC);
        return 1;
    } else {
        printf("Note:  strDesIP: %s does not use\n", strDesIP);
        return 0;
    }
}


// 获取本地某网卡的MAC
// In: strEth
// Out: 若成功，返回MAC字符串，失败，返回""(空串)
char *GCArp::GetLocalMac(char *strEth) {

    int s;
    struct ifreq buffer;
    char chBuff[MAX_MAC_LEN];
    unsigned int str_len = 0;
    unsigned int offset = 0;

    memset(chBuff, 0x0, sizeof(chBuff));
    //arp_process(NULL);
    s = socket(PF_INET, SOCK_DGRAM, 0);
    if (-1 == s) {
        wenxy_debug("Error: create socket failture\n");
        wenxy_debug("--------------------------------\n\n");
        return "";
    }
    memset(&buffer, 0x00, sizeof(buffer));
    strcpy(buffer.ifr_name, strEth);  // "eth0"
    if (-1 == ioctl(s, SIOCGIFHWADDR, &buffer)) {
        wenxy_debug("Error: 获取接口 %s MAC 失败\n", strEth);
        wenxy_debug("--------------------------------\n\n");
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
        //wenxy_debug("%.2X", (unsigned char)buffer.ifr_hwaddr.sa_data[s]);
        if (s < 5) {
            memset(chBuff, 0x0, sizeof(chBuff));
            sprintf(chBuff, "%s", COMPART_MAC);
            str_len = strlen(chBuff);
            memcpy(strLocalMAC + offset, chBuff, str_len);
            offset += str_len;
            //wenxy_debug(":");
        }
    }
    //wenxy_debug("\n");
    return strLocalMAC;
}


//-------------------------------------------------------
// 发送ARP包，并接收ARP应答包，取出MAC
// In: strSrcIP:本机IP，strSrcMAC:本机IP的MAC，strDesIP:被请求应答MAC的IP , strNIC:本地接口名
// Out: 若成功，返回MAC，失败返回""(空串)
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
    int i;
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

#define SRC_IP   chSrcIP  // 源IP
#define DES_IP   chDesIP  // 目的IP
#define LOCAL_HW        chSrcMAC // eth0的MAC
#define DEVICE   chNIC  // 本机接口名

    memset(&arp, 0x00, sizeof(arp));
    memset(&arpRes, 0x00, sizeof(arpRes));
#if 1
    wenxy_debug("源IP[%s]  源MAC[%s] 目的IP[%s]\n", strSrcIP, strSrcMAC, strDesIP);
#endif

    sockfd = socket(AF_INET, SOCK_PACKET, htons(0x0806));
    if (sockfd < 0) {
        wenxy_debug("Error: create socket failed\n");
        wenxy_debug("--------------------------------\n\n");
        return "";
    }

    /*
    // 设置socket为非阻塞模式
    if ( -1 != fcntl(sockfd, F_SETFL, O_NONBLOCK) )
    {
    wenxy_debug("设置socket为非阻塞模式成功\n");
    }
    else
    {
    wenxy_debug("Warning: 设置socket为非阻塞模式失败[errno = %d]\n", errno);
    }
    */

    // 设置socket接收超时
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100;
    if (0 == setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))) {
        wenxy_debug("设置socket接收超时成功\n");
    } else {
        wenxy_debug("Warning: 设置socket接收超时失败[errno = %d]\n", errno);
    }
#if 1
    wenxy_debug("创建socket SOCK_PACKET 成功\n");
    wenxy_debug("Note: 创建ARP请求包 ......\n");
    wenxy_debug("--------------------------------\n\n");
#endif

    // 创建ARP请求包
    /* init arp packet header */
    arp.ethhdr.ether_type = htons(FRAME_TYPE);
    set_hw_addr((char *) arp.ethhdr.ether_dhost, DES_MAC);
    set_hw_addr((char *) arp.ethhdr.ether_shost, LOCAL_HW);
#if 1
    wenxy_debug("%x|", arp.ethhdr.ether_type);
    for (i = 0; i < 6; i++) {
        wenxy_debug("%d_", arp.ethhdr.ether_dhost[i]);
    }
    wenxy_debug("|");
    for (i = 0; i < 6; i++) {
        wenxy_debug("%d_", arp.ethhdr.ether_shost[i]);
    }
    wenxy_debug("\n--------------------------------\n");
    wenxy_debug("初始化ARP包帧头(以太网首部)成功\n\n");
#endif

    /* init arp packet data */
    wenxy_debug("初始化ARP包帧数据(ARP请求分组) ......\n");
    wenxy_debug("--------------------------------\n");
    arp.arphdr.ar_hrd = htons(HARD_TYPE); // 1
    arp.arphdr.ar_pro = htons(PROTO_TYPE); // 0x0800
    arp.arphdr.ar_op = htons(OP_CODE);  // 1
    arp.arphdr.ar_hln = (unsigned char) (6);
    arp.arphdr.ar_pln = (unsigned char) (4);
#if 1
    wenxy_debug("%d|%d|%d|%d|%d|\n", arp.arphdr.ar_hrd, arp.arphdr.ar_pro, arp.arphdr.ar_op, arp.arphdr.ar_hln,
                arp.arphdr.ar_pln);
    wenxy_debug("--------------------------------\n");
#endif

    set_hw_addr((char *) arp.arphdr.__ar_tha, DES_MAC); // 请求IP的MAC
    set_hw_addr((char *) arp.arphdr.__ar_sha, LOCAL_HW); // 发送者的MAC
    set_ip_addr((char *) arp.arphdr.__ar_tip, DES_IP); // 请求MAC的IP
    set_ip_addr((char *) arp.arphdr.__ar_sip, SRC_IP); // 源IP
    bzero(arp.padding, 18); // 填充18个字节
#if 1
    for (i = 0; i < 6; i++) {
        wenxy_debug("%d_", arp.arphdr.__ar_sha[i]);
    }
    wenxy_debug("|");
    for (i = 0; i < 6; i++) {
        wenxy_debug("%d_", arp.arphdr.__ar_sip[i]);
    }
    wenxy_debug("|");
    for (i = 0; i < 6; i++) {
        wenxy_debug("%d_", arp.arphdr.__ar_tha[i]);
    }
    wenxy_debug("|");

    for (i = 0; i < 6; i++) {
        wenxy_debug("%d_", arp.arphdr.__ar_tip[i]);
    }
    wenxy_debug("|");
    wenxy_debug("\n--------------------------------\n");

#endif

    /* send arp reply packet */
    memset(&sa, 0x00, sizeof(sa));
    strcpy(sa.sa_data, DEVICE);

    // 发送ARP包
    int nSendCount = ARP_SEND_COUNT;
    int nRecvByte = 0;

    while ((nSendCount--) > 0) {

        wenxy_debug("发送ARP请求包[%d Bytes]...... [第%d个]\n", sizeof(arp), nSendCount);
        if (sendto(sockfd, &arp, sizeof(arp), 0, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
            wenxy_debug("Error: 发送ARP包失败 [errno = %d]\n", errno);
            return "";
        }

        // 接收ARP应答包
        wenxy_debug("Note: 接收ARP应答 ......\n");
        int nTryCount = RX_ARP_COUNT;
        int nAddrLen = sizeof(sa);

        do {
            /* because network and host delay */
            usleep(SLEEP_MAX_US);

            nRecvByte = recvfrom(sockfd, &arpRes, sizeof(arpRes), 0, (struct sockaddr *) &sa, (socklen_t *) &nAddrLen);

            // 若是所请求IP的ARP应答包，退出while
            if (nRecvByte >= 60 && 2 == ntohs(arpRes.arphdr.ar_op)) {
                char chBuff[MAX_MAC_LEN];
                // 格式化IP
                offset = 0;
                for (s = 0; s < 4; s++) {
                    memset(chBuff, 0x00, sizeof(chBuff));
                    sprintf((char *) chBuff, "%d", (unsigned char) arpRes.arphdr.__ar_sip[s]);
                    //wenxy_debug("chBuff: %s\n", chBuff);
                    //strTarIP += chBuff;
                    str_len = strlen(chBuff);
                    memcpy(strTarIP + offset, chBuff, str_len);
                    offset += str_len;
                    if (s < 3) {
                        memset(chBuff, 0x00, sizeof(chBuff));
                        sprintf((char *) chBuff, "%s", ".");
                        //strTarIP += chBuff;
                        str_len = 1;
                        memcpy(strTarIP + offset, chBuff, str_len);
                        offset += str_len;
                    }
                }

                if (!strcmp(strTarIP, strDesIP)) {
                    wenxy_debug("\n请求IP[%s] = 应答IP[%s]\n", strDesIP, strTarIP);
                    find_ip_flag = 1; /* find ip */
                    goto analyse_arp_response;
                }
            }

        } while (nTryCount--); // 接收ARP应答包的次数

    }

    analyse_arp_response:
    wenxy_debug("已接收到ARP应答包 [%d Bytes]\n", nRecvByte);
    // 接收超时，或错误
    if (nRecvByte == -1) {
        wenxy_debug("Warning: 接收超时，或计算机[%s]没有响应\n", strDesIP);
        close(sockfd);
        return "";
    }

    wenxy_debug("分析ARP应答包 ......\n");
    char chBuff[MAX_MAC_LEN];
    memset(chBuff, 0x00, sizeof(chBuff));
    // 格式化IP
    offset = 0;
    for (s = 0; s < 4; s++) {
        memset(chBuff, 0x00, sizeof(chBuff));
        sprintf((char *) chBuff, "%d", (unsigned char) arpRes.arphdr.__ar_sip[s]);
        //strTarIP += chBuff;
        str_len = strlen(chBuff);
        memcpy(strTarIP + offset, chBuff, str_len);
        offset += str_len;
        if (s < 3) {
            memset(chBuff, 0x00, sizeof(chBuff));
            sprintf((char *) chBuff, "%s", ".");
            //strTarIP += chBuff;
            str_len = strlen(chBuff);
            memcpy(strTarIP + offset, chBuff, str_len);
            offset += str_len;
        }
    }
    // 格式化MAC
    memset(chBuff, 0x00, sizeof(chBuff));
    offset = 0;
    for (s = 0; s < 6; s++) {
        memset(chBuff, 0x00, sizeof(chBuff));
        sprintf((char *) chBuff, "%02X", (unsigned char) arpRes.arphdr.__ar_sha[s]);
        //strTarMAC += chBuff;
        str_len = strlen(chBuff);
        memcpy(strTarMAC + offset, chBuff, str_len);
        offset += str_len;
        if (s < 5) {
            memset(chBuff, 0x00, sizeof(chBuff));
            sprintf((char *) chBuff, "%s", COMPART_MAC);
            //strTarMAC += chBuff;
            str_len = strlen(chBuff);
            memcpy(strTarMAC + offset, chBuff, str_len);
            offset += str_len;
        }
    }
    // 输出目的IP，目的MAC
    wenxy_debug("应答IP[%s] 对应的MAC[%s]\n", strTarIP, strTarMAC);
    wenxy_debug("\n--------------------------------\n\n");
    close(sockfd);
    // 返回被请求的MAC
    strcpy(strDesMAC, strTarMAC);
    /* return */
    return strDesMAC;
}


// 填充MAC
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


// 填充IP
void GCArp::set_ip_addr(char *buf, char *str) {

    struct in_addr addr;
    memset(&addr, 0x00, sizeof(addr));
    addr.s_addr = inet_addr(str);
    memcpy(buf, &addr, 4);
}
