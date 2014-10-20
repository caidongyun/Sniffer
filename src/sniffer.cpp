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
    this->pAllNetDevs = NULL;
    this->iNetDevsNum = 0;
    this->device = NULL;
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
    if (this->pAllNetDevs)
    {
        pcap_freealldevs(this->pAllNetDevs);
        this->pAllNetDevs = NULL;
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

    if (pcap_findalldevs(&(this->pAllNetDevs), this->errbuf) == -1)
    {
        qWarning("%s", this->errbuf);
        return false;
    }

    qDebug("Get device");
    NetDevInfo tmpNetInfo;
    for (pcap_if_t *ind = this->pAllNetDevs; ind != NULL; ind = ind->next)
    {
        tmpNetInfo.strDevName = ind->name;
        tmpNetInfo.strDevDesc = ind->description;

        this->netDevsInfoVector.append(tmpNetInfo);
        qDebug("device%d:%s", this->iNetDevsNum, ind->name);
        this->iNetDevsNum ++;
    }
    qDebug("Get %d devices", this->iNetDevsNum);
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
    if (this->device != NULL)
    {
        closeNetDev();
    }

    device = pcap_open_live(szDevName,     // The device name
                            iLengthLimit,  // The max length of the packet data
                            flag,          // The open mode, If 0 means none Promiscuous Mode, otherwises
                            PCAP_TIMEOUT,  // The timeout of the function
                            this->errbuf
            );

    if (device != NULL)
    {
        return true;
    }
    else
    {
        qWarning("%s", this->errbuf);
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
    if (this->device != NULL)
    {
        closeNetDev();
        this->device = NULL;
    }

    if (iDevNum < 0 || iDevNum > this->iNetDevsNum)
    {
        return false;
    }

    pcap_if_t* ind = this->pAllNetDevs;
    for (int i = 0; i < iDevNum; i++)
    {
        ind = ind->next;
    }


    device = pcap_open_live(ind->name,     // The device name
                            iLengthLimit,  // The max length of the packet data
                            flag,          // The open mode, If 0 means none Promiscuous Mode, otherwises
                            PCAP_TIMEOUT,  // The timeout of the function
                            this->errbuf
            );

    if (device != NULL)
    {
        return true;
    }
    else
    {
        qWarning("%s", this->errbuf);
        return false;
    }
}

bool Sniffer::closeNetDev()
{
    if (this->device != NULL)
    {
        pcap_close(this->device);
        this->device = NULL;
        return true;
    }
    return false;
}

int Sniffer::captureOnce()
{
   int res = pcap_next_ex(this->device, &(this->header), &(this->packetData));

   return res;
}

QString Sniffer::checkBnfExpr(int devIndex,const QString& expr)
{
    int flag = 0;
    if (this->device == NULL)
    {
        if(!openNetDev(devIndex))
        {
            return "Can not open device";
        }
        flag = 1;
    }
    struct bpf_program fcode;
    if (pcap_compile(this->device, &fcode, 
                expr.toStdString().data(), 1, PCAP_NETMASK_UNKNOWN) != 0)
    {
        return QString(pcap_geterr(this->device));
    }
    if (flag == 1)
    {
        closeNetDev();
    }
    return NULL;
}

bool Sniffer::setDeviceFilter(const QString& expr)
{

    struct bpf_program fcode;
    if (pcap_compile(this->device, &fcode, 
                expr.toStdString().data(), 1, PCAP_NETMASK_UNKNOWN) != 0)
    {
        return false;
    }
    
    if (pcap_setfilter(this->device, &fcode) < 0)
    {
        return false;
    }

    return true;
}
