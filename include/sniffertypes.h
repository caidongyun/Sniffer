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
#include <QtCore/QVector>

#include <pcap.h>

#include <time.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class SnifferProtocal
{
    public:
        QString strProtocal;    // The protocal name like ip, ether
        void* pProtocal;        // The protocal data struct, such as ip protocal 
                                // it pointer to the struct ip 
};


struct SnifferData
{
    public:
        struct pcap_pkthdr* header;     // the header of the captured data
        QByteArray rawData;             // The raw data
        QVector<SnifferProtocal*> protocalVec;
        //DetailPacketAnalysis detailAnalysis;
    
        SnifferData()
        {
            rawData = NULL;
            header = NULL;
        };

        void reInit()
        {
            rawData = NULL;
            header = NULL;
            protocalVec.clear();
        }
};


namespace SnifferType
{
    const QString ETHER_PROTOCAL = "ether";
    const QString IP_PROTOCAL = "ip";
    const QString IPV6_PROTOCAL = "ipv6";
    const QString ARP_PROTOCAL = "arp";
    const QString TCP_PROTOCAL = "tcp";
    const QString UDP_PROTOCAL = "udp";
    const QString HTTP_PROTOCAL = "http";
    const QString SSDP_PROTOCAL = "ssdp";
}

// The application layer struct
class ApplicationData
{
    public:
        QString strProtocal;     // Note that the QString is 2 bytes
        QString strContent;      // The content 
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
