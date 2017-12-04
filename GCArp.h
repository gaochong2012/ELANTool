//
// Created by gaochong on 17-11-24.
//

#ifndef ELANTOOL_GCARP_H
#define ELANTOOL_GCARP_H

#include <unistd.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <string.h>
#include <errno.h>

#include<netinet/in.h>
#include<arpa/inet.h>
#include<ctype.h>
#include <fcntl.h>

#define MAX_MAC_LEN           24          // MAC字串缓冲区的最大长度(byte)
#define COMPART_MAC           ":"         // MAC字串的分格符, Warnning:修改此宏，必须再修改源程序!!!

#define MAX_BUF_LEN           128

#define PADD_MAC              "00:00:00:00:00:00" 	 // 填充的MAC
#define DES_MAC               "FF:FF:FF:FF:FF:FF" 	 // 广播MAC
//#define ARP_SEND_COUNT        1				     // 发送ARP请求的ARP包的个数
//#define RX_ARP_COUNT          1				     // rx count of ARP response

//#define SLEEP_MAX_US ( 1000 )		                 // unit microsecond */

struct ether_header {
    unsigned char  ether_dhost[6];          /* destination eth addr */
    unsigned char  ether_shost[6];          /* source ether addr    */
    unsigned short ether_type;              /* packet type ID field */
};

struct arp_header {
    unsigned short int ar_hrd;              /* Format of hardware address.  */
    unsigned short int ar_pro;              /* Format of protocol address.  */
    unsigned char ar_hln;                   /* Length of hardware address.  */
    unsigned char ar_pln;                   /* Length of protocol address.  */
    // -------------------------
    unsigned short int ar_op;               /* ARP opcode (command).  */
    unsigned char __ar_sha[6];              /* Sender hardware address.  */
    unsigned char __ar_sip[4];              /* Sender IP address.  */
    unsigned char __ar_tha[6];              /* Target hardware address.  */
    unsigned char __ar_tip[4];              /* Target IP address.  */
    // -------------------------
};

struct arp_packet {

    struct ether_header ethhdr;
    struct arp_header arphdr;
    unsigned char padding[18];              /* filled with 0 */
};

/**
* arp reply:
*     op = 2
*     ethhdr.ether_dhost = arphdr.__ar_tha = switch hard addr
*     ethhdr.ether_shost = arphdr.__ar_sha = local hard addr
*     arphdr.__ar_tip = switch ip
*     arphdr.__ar_sip = victim ip
*/
#define FRAME_TYPE      0x0806                  /* arp=0x0806,rarp=0x8035 */
#define HARD_TYPE       1                       /* ethernet is 1 */
#define PROTO_TYPE      0x0800                  /* IP is 0x0800 */
#define OP_CODE         1                       /* arp=1/2,1为请求，2为应答,rarp=3/4 */

class GCArp {

    public:

    GCArp();

    GCArp(int srCount, int outTime);

    ~GCArp();
        const char* GetRemoteIPMAC(  const char *nic_name,  const char *nic_ip, const char *des_ip );

    private:

        char strETH[MAX_BUF_LEN] = {0};  	  // 本机NIC名称
        char strLocalMAC[MAX_BUF_LEN] = {0};  // 本机MAC
        char strSrcIP[MAX_BUF_LEN] = {0}; 	  // 本机IP

        char strDesMAC[MAX_BUF_LEN] = {0};
        char strDesIP[MAX_BUF_LEN] = {0}; 	  // 目的IP

        char strTarIP[MAX_BUF_LEN] = {0};     // Target IP address
        char strTarMAC[MAX_BUF_LEN] = {0};    // Target hardware address

        int  find_ip_flag = 0;	              // 1 means find, 0 means not find */

        char* GetLocalMac(char *strEth);          // get loacl NIC's MAC
        void  set_ip_addr(char *,char *);         // 填充IP
        void  set_hw_addr(char buf[], char *str); // 填充MAC
        char* GetMacByIP(char *strSrcIP, char *strSrcMAC, char *strDesIP ,char *strNIC ); // 获取指定IP的MAC
        int arp_main( const char *nic_name, const char *nic_ip, const char *des_ip );


        int srCount  = 1;
        int outTime  = 250;

};


#endif //ELANTOOL_GCARP_H
