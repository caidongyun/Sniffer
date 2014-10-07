/*
 * =====================================================================================
 *
 *       Filename:  sniffertypes.h
 *
 *    Description:  Define some types used in Sniffer
 *
 *        Version:  1.0
 *        Created:  2014年09月21日 22时50分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#ifndef SNIFFERTYPES_H_
#define SNIFFERTYPES_H_

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//// Mac header (14 bytes)
//typedef struct _ethHeader
//{
//    unsigned char dstmac[6];    // 目标mac地址  
//    unsigned char srcmac[6];    // 来源mac地址  
//    unsigned short eth_type;    // 以太网类型  
//}ethHeader, *pEthHeader;
//
//// Arp header(28 bytes)
//typedef struct _arpHeader{
//    unsigned short arp_hdr;   /*ARP分组中的硬件类型，2字节，定义运行ARP的网络的类型，以太网是类型1*/
//    unsigned short arp_pro;  /*协议类型，2字节，定义上层协议类型，对于IPV4协议，该字段值为0800*/
//    unsigned char arp_hln;  /*硬件长度，8位字段，定义对应物理地址长度，以太网中这个值为6*/
//    unsigned char apr_pln; /*协议长度，8位字段，定义以字节为单位的逻辑地址长度，对IPV4协议这个值为4*/
//    unsigned  short arp_opt;  /*16位字段，定义分组类型，是ARP请求（值为1），或者ARP应答（值为2）*/
//    unsigned char arp_sha[6]; /*发送端硬件地址，可变长度字段，对以太网这个字段是6字节长*/
//    unsigned char arp_spa[4]; /*发送端协议地址，可变长度字段，对IP协议，这个字段是4字节长*/
//    unsigned char arp_tha[6]; /*接受端硬件地址*/
//    unsigned char arp_tpa[4];/*接收端协议地址*/
//
//} arpHeader, *pArpHeader;
//
//// IpV4 header(20 bytes)
//typedef struct _ipHeader
//{
//    unsigned char       ip_hl:4, ip_v:4;        // 版本 (4 bits) + 首部长度 (4 bits)
//    unsigned char       tos;            // 服务类型(Type of service) 
//    unsigned short      tlen;           // 总长(Total length) 
//    unsigned short      identification; // 标识(Identification)
//    unsigned short      flags_fo;       // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
//    unsigned char       ttl;            // 存活时间(Time to live)
//    unsigned char       proto;          // 协议(Protocol)
//    unsigned short      crc;            // 首部校验和(Header checksum)
//    unsigned char       saddr[4];       // 源地址(Source address)
//    unsigned char       daddr[4];       // 目标地址(Destination address)
//} ipHeader, *pIpHeader;
//
//// Tcp header(20 bytes)
//typedef struct _tcpHeader
//{
//    unsigned short  sport;              // 源端口号
//    unsigned short  dport;              // 目的端口号
//    unsigned int    seq_no;             // 序列号
//    unsigned int    ack_no;             // 确认号
//    unsigned char   thl:4;              // tcp头部长度
//    unsigned char   reserved_1:4;       // 保留6位中的4位首部长度
//    unsigned char   reseverd_2:2;       // 保留6位中的2位
//    unsigned char   flag:6;             // 6位标志 
//    unsigned short  wnd_size;           // 16位窗口大小
//    unsigned short  chk_sum;            // 16位TCP检验和
//    unsigned short  urgt_p;             // 16为紧急指针
//} tcpHeader, pTcpHeader;
//
//// Udp header(8 bytes)
//typedef struct _udpHeader
//{
//    unsigned short  sport;      // 源端口(Source port)
//    unsigned short  dport;      // 目的端口(Destination port)
//    unsigned short  len;        // UDP数据包长度(Datagram length)
//    unsigned short  crc;        // 校验和(Checksum)
//} udpHeader, *pUdpHeader;
//

//typedef struct _detailPacketAnalysis
//{
//    struct ether_header *pEtherHeader; // The mac header
//    union
//    {
//        struct _ip_data
//        {
//            struct ip *pIpHeader;
//            union 
//            {
//                struct icmphdr *pIcmpHeader;
//                union 
//                {
//                    struct tcphdr *pTcpHeader;
//                    struct udphdr *UdpHeader;
//                } tcpUdpHeader;
//            } netData;
//        } ipData;
//        struct ether_arp *pArpHeader;
//    } netHeader; // The net header, including the arp header and ip header.
//} DetailPacketAnalysis, *PDetailPacketAnalysis;

class SnifferData
{
    public:
        QString strTime;      // The time that the packet been sniffer
        QString strSrc;       // The source 
        QString strDst;       // The destination 
        QString strProtocol;  // The protocal
        int iLength;          // The length of tha data
        QString info;         // Some short info about the data
        QByteArray rawData;   // The raw data
        //DetailPacketAnalysis detailAnalysis;
        
        void reInit()
        {
            strTime = "";
            strSrc = "";
            strDst = "";
            strProtocol = "";
            iLength = 0;
            info = "";
            rawData = NULL;
        }
};

// TCP protocal
#define FTP_PORT        (21)
#define TELNET_PORT     (23)
#define SMTP_PORT       (25) 
#define HTTP_PORT       (80) 
#define HTTPS_PORT      (443) 
#define POP3_PORT       (110)

// UDP protocal
#define DNS_PORT        (53)
#define SNMP_PORT       (161)

#endif
