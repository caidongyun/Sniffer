/*
 * =====================================================================================
 *
 *       Filename:  sniffer.cpp
 *
 *    Description:  网络嗅探类的主文件
 *
 *        Version:  1.0
 *        Created:  2014年09月21日 20时22分45秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#include <pcap.h>

#include "../include/sniffer.h"


Sniffer::Sniffer()
{
    Sniffer::pAllNetDevs = NULL;
    Sniffer::iNetDevsNum = 0;
}

Sniffer::~Sniffer()
{
    freeNetDevsMem();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  freeNetDevsMem
 *  Description:  Free the net device memory
 * =====================================================================================
 */
void Sniffer::freeNetDevsMem()
{
    if (Sniffer::pAllNetDevs)
    {
        pcap_freealldevs(Sniffer::pAllNetDevs);
        Sniffer::pAllNetDevs = NULL;
    }
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getAllNetDevs
 *  Description:  Get all the net device and store into the pAllNetDevs
 * =====================================================================================
 */
bool Sniffer::getAllNetDevs()
{
    freeNetDevsMem();

    if (pcap_findalldevs(&(Sniffer::pAllNetDevs), Sniffer::errbuf) == -1)
    {
        qWarning("%s", Sniffer::errbuf);
        return false;
    }

    qDebug("Get device");
    for (pcap_if_t *ind = Sniffer::pAllNetDevs; ind != NULL; ind = ind->next)
    {
        qDebug("device%d:%s", Sniffer::iNetDevsNum, ind->name);
        Sniffer::iNetDevsNum ++;
    }
    qDebug("Get %d devices", Sniffer::iNetDevsNum);
    return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  openNetDev
 *  Description:  open the net device
 *  Parameters:
 *                szDevName: The device name
 *                flag: The open mode
 *                iLengthLimit: The max length of the packet data
 * =====================================================================================
 */
bool Sniffer::openNetDev(char *szDevName, int flag, int iLengthLimit)
{
    if (Sniffer::device != NULL)
    {
        closeNetDev();
    }

    device = pcap_open_live(szDevName,     // The device name
                            iLengthLimit,  // The max length of the packet data
                            flag,          // The open mode, If 0 means none Promiscuous Mode, otherwises
                            PCAP_TIMEOUT,  // The timeout of the function
                            Sniffer::errbuf
            );

    if (device != NULL)
    {
        return true;
    }
    else
    {
        qWarning("%s", Sniffer::errbuf);
        return false;
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  openNetDev
 *  Description:  open the net device
 *  Parameters:
 *                iDevNum: The ith device
 *                flag: The open mode
 *                iLengthLimit: The max length of the packet data
 * =====================================================================================
 */
bool Sniffer::openNetDev(int iDevNum, int flag, int iLengthLimit)
{
    if (Sniffer::device != NULL)
    {
        closeNetDev();
    }

    if (iDevNum < 0 || iDevNum > Sniffer::iNetDevsNum)
    {
        return false;
    }

    pcap_if_t* ind = Sniffer::pAllNetDevs;
    for (int i = 0; i < iDevNum; i++)
    {
        ind = ind->next;
    }


    device = pcap_open_live(ind->name,     // The device name
                            iLengthLimit,  // The max length of the packet data
                            flag,          // The open mode, If 0 means none Promiscuous Mode, otherwises
                            PCAP_TIMEOUT,  // The timeout of the function
                            Sniffer::errbuf
            );

    if (device != NULL)
    {
        return true;
    }
    else
    {
        qWarning("%s", Sniffer::errbuf);
        return false;
    }
}

bool Sniffer::closeNetDev()
{
    if (Sniffer::device != NULL)
    {
        pcap_close(Sniffer::device);
        Sniffer::device = NULL;
        return true;
    }
    return false;
}

int Sniffer::captureOnce()
{
   int res = pcap_next_ex(Sniffer::device, &(Sniffer::header), &(Sniffer::packetData));
   qDebug("Time:%s---data:%s", ctime((const time_t*)&Sniffer::header->ts.tv_sec), Sniffer::packetData);
   return res;
}
