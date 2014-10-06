/*
 * =====================================================================================
 *
 *       Filename:  sniffer.h
 *
 *    Description:  网络嗅探类的头文件
 *
 *        Version:  1.0
 *        Created:  2014年09月21日 20时24分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */

#ifndef SNIFFER_H_
#define SNIFFER_H_
#include <QtCore/QtDebug>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QVector>

#include <pcap.h>
#include <time.h>

#include "sniffertypes.h"

#define PCAP_PROMISCUOUS 1  // Open the device in Promiscuous Mode
#define PCAP_TIMEOUT 1000   // The timeout of pcap

class Sniffer
{
    public:
        Sniffer();
        ~Sniffer();

        void freeNetDevsMem();  // Free the net devices memory
        bool getAllNetDevs();  // Get all the net devices
        bool openNetDev(char *szDevName,              // The name of the device
                        int flag = PCAP_PROMISCUOUS,  // If open the device in promiscuous mode
                        int iLengthLimit = 65536      // The max legth which should be capture, 
                                                      // The data in every protocal should less then 65535
                        );
        bool openNetDev(int iDevNum,                  // The ith device
                        int flag = PCAP_PROMISCUOUS,
                        int iLengthLimit = 65536
                        );
        bool closeNetDev();  // Close the opened device

        int captureOnce();   // Capture once


    public:
        pcap_if_t *pAllNetDevs;         // The pointer of all the net device list
        int iNetDevsNum;                // The total number of the net device
        char errbuf[PCAP_ERRBUF_SIZE];  // The error message buffer, which contains the error message
        pcap_t *device;                 // The net device
        struct pcap_pkthdr* header;     // The captured packet header
        const u_char *packetData;       // The data packet 
        QVector<SnifferData> snifferDataVector; // The sniffered Data
        
};
#endif
