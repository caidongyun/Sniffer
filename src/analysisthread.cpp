/*
 * =====================================================================================
 *
 *       Filename:  analysisthread.cpp
 *
 *    Description:  For the more, see ../include/analysisthread.h
 *
 *        Version:  1.0
 *        Created:  2014年10月11日 19时50分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#include "../include/analysisthread.h"

AnalysisThread::AnalysisThread()
{
}

AnalysisThread::AnalysisThread(QStandardItemModel* model)
{
    this->model = model;
    this->irow = 0;
}

AnalysisThread::~AnalysisThread()
{

}

void AnalysisThread::anslysisData(const SnifferData& snifferData, int frameIndex)
{
    this->model->clear();
    this->irow = 0;

    this->snifferData = snifferData;
    addFrameNode(frameIndex);
}

void AnalysisThread::addFrameNode(int frameIndex)
{
    QStandardItem *frameitem = new QStandardItem();
    frameitem->setText(QString("Frame %1: %2 bytes on wire (%3 bits),"
                "%4 bytes captured (%5 bits)"
                ).arg(frameIndex)
                 .arg(snifferData.header->len)
                 .arg(snifferData.header->len * 8)
                 .arg(snifferData.header->caplen)
                 .arg(snifferData.header->caplen * 8));

    QStandardItem *item = new QStandardItem();
    item->setText(QString("Encapsulation: %1").arg("Ethernet"));
    frameitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Arrival time: %1 CST").
            arg(SnifferUtil::getFormatTimeCST(&snifferData.header->ts.tv_sec)));
    frameitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Frame Number: %1").arg(frameIndex));
    frameitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Frame Length: %1 bytes (%2 bits)").
            arg(snifferData.header->len).arg(snifferData.header->len * 8));
    frameitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Capture Length: %1 bytes (%2 bits)").
            arg(snifferData.header->caplen).arg(snifferData.header->caplen * 8));
    frameitem->appendRow(item);

    // The Frame outline tree node
    this->model->setItem(this->irow++,0, frameitem);
    this->addEtherNode(0);
}

void AnalysisThread::addEtherNode(int protoInd)
{
    SnifferProtocal* psp = this->snifferData.protocalVec.at(protoInd);
    struct ether_header* pether_header = (struct ether_header *)psp->pProtocal;
    QStandardItem* etheritem = new QStandardItem();
    
    QString tmpmacDst = SnifferUtil::macToHost(pether_header->ether_dhost);
    QString tmpmacSrc = SnifferUtil::macToHost(pether_header->ether_shost);
    etheritem->setText(QString("Ethernet V2, Src: %1, Dst: %2")
            .arg(tmpmacSrc)
            .arg(tmpmacDst));


    // Destination
    QStandardItem* item = new QStandardItem();
    item->setText("Destination: " + tmpmacDst);
    
    QStandardItem* childitem = new QStandardItem();
    childitem->setText("Address: " + tmpmacDst);
    item->appendRow(childitem);

    childitem = new QStandardItem();

    if (SnifferUtil::getBit(pether_header->ether_dhost[1], 6))
    {
        childitem->setText(QString(".... ..1. .... .... .... .... = LG bit: Locally administered address (this is NOT the factory default)"));
    }
    else
    {
        childitem->setText(QString(".... ..0. .... .... .... .... = LG bit: Globally unique address (factory default)"));
    }
    item->appendRow(childitem);

    childitem = new QStandardItem();
    if (SnifferUtil::getBit(pether_header->ether_dhost[1], 7))
    {
        childitem->setText(QString(".... ...1 .... .... .... .... = IG bit: Group address (multicast/broadcast)"));
    }
    else
    {
        childitem->setText(QString(".... ...0 .... .... .... .... = IG bit: Individual address (unicast)"));
    }

    item->appendRow(childitem);
    etheritem->appendRow(item);

    // Source 
    item = new QStandardItem();
    item->setText("Source: " + tmpmacSrc);
    
    childitem = new QStandardItem();
    childitem->setText("Address: " + tmpmacSrc);
    item->appendRow(childitem);

    childitem = new QStandardItem();

    if (SnifferUtil::getBit(pether_header->ether_shost[1], 6))
    {
        childitem->setText(QString(".... ..1. .... .... .... .... = LG bit: Locally administered address (this is NOT the factory default)"));
    }
    else
    {
        childitem->setText(QString(".... ..0. .... .... .... .... = LG bit: Globally unique address (factory default)"));
    }
    item->appendRow(childitem);

    childitem = new QStandardItem();
    if (SnifferUtil::getBit(pether_header->ether_shost[1], 7))
    {
        childitem->setText(QString(".... ...1 .... .... .... .... = IG bit: Group address (multicast/broadcast)"));
    }
    else
    {
        childitem->setText(QString(".... ...0 .... .... .... .... = IG bit: Individual address (unicast)"));
    }

    item->appendRow(childitem);
    etheritem->appendRow(item);

    // Get the protocal type
    int type = ntohs(pether_header->ether_type); 

    switch(type)
    {
        case ETHERTYPE_IP:
        {
            item = new QStandardItem();
            item->setText(QString("Type: IP (0x%1)").arg(type,4, 16,QLatin1Char('0')));
            etheritem->appendRow(item);
            this->model->setItem(this->irow++, 0, etheritem);   

            addIpNode(protoInd+1);
            break;
        }
        case ETHERTYPE_IPV6:
        {
            item = new QStandardItem();
            item->setText(QString("Type: IPV6 (0x%1)").arg(type,4, 16,QLatin1Char('0')));
            etheritem->appendRow(item);
            this->model->setItem(this->irow++, 0, etheritem);   

            addIpV6Node(protoInd+1);
            break;
        }
        case ETHERTYPE_ARP:
        {
            item = new QStandardItem();
            item->setText(QString("Type: ARP (0x%1)").arg(type,4, 16,QLatin1Char('0')));
            etheritem->appendRow(item);
            this->model->setItem(this->irow++, 0, etheritem);   

            addArpNode(protoInd+1);
            break;
        }
        default:
        {
            qDebug("Current the sniffer can not analysis the ethernet type:%0x", type);
            break;
        }
    }
}

void AnalysisThread::addIpNode(int protoInd)
{
    struct ip* pip_header = (struct ip *)this->snifferData.protocalVec.at(protoInd)->pProtocal;

    QStandardItem* ipitem = new QStandardItem();
    ipitem->setText(QString("Internet Protocol Version 4, Src: %1, Dst: %2")
            .arg(inet_ntoa(pip_header->ip_src))
            .arg(inet_ntoa(pip_header->ip_dst)));

    QStandardItem* item = new QStandardItem();
    item->setText("Version: 4");
    ipitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Header length: %1 bytes").arg(pip_header->ip_hl));
    ipitem->appendRow(item);

    // ECN(last 2 bits) and DSCP(first 6 bits) type
    item = new QStandardItem();
    item->setText(QString("Differentiated Services Field: 0x%1 (DSCP 0x%2: %3; ECN: 0x%4: %5)")
            .arg(SnifferUtil::byteToHexStr(pip_header->ip_tos,16))
            .arg(SnifferUtil::byteToHexStr(IPTOS_DSCP(pip_header->ip_tos),16))
            .arg(SnifferUtil::getDSCPDesc(pip_header->ip_tos))
            .arg(SnifferUtil::byteToHexStr(IPTOS_ECN(pip_header->ip_tos),16))
            .arg(SnifferUtil::getECNDesc(pip_header->ip_tos)));

    // DSCP
    QStandardItem* childitem = new QStandardItem();
    childitem->setText(QString("%1.. = Differentiated Services Codepoint: %2 (0x%3)")
            .arg(SnifferUtil::byteToHexStr(pip_header->ip_tos,2).left(6))
            .arg(SnifferUtil::getDSCPDesc(pip_header->ip_tos))
            .arg(SnifferUtil::byteToHexStr(IPTOS_DSCP(pip_header->ip_tos),16)));
    item->appendRow(childitem);

    // ECN
    childitem = new QStandardItem();
    childitem->setText(QString(".... ..%1 = Explicit Congestion Notification: %2 (0x%3)")
            .arg(SnifferUtil::byteToHexStr(pip_header->ip_tos,2).right(2))
            .arg(SnifferUtil::getECNDesc(pip_header->ip_tos))
            .arg(SnifferUtil::byteToHexStr(IPTOS_ECN(pip_header->ip_tos),16)));
    item->appendRow(childitem);
    
    ipitem->appendRow(item);

    // Total length and Indentification
    item = new QStandardItem();
    item->setText(QString("Total Length: %1").arg(ntohs(pip_header->ip_len)));
    ipitem->appendRow(item);
    item = new QStandardItem();
    item->setText(QString("Identification: 0x%1 (%2)")
            .arg(ntohs(pip_header->ip_id),0,16)
            .arg(ntohs(pip_header->ip_id)));
    ipitem->appendRow(item);

    // ip Flag
    item = new QStandardItem();
    short flag = pip_header->ip_off & (0xffff - IP_OFFMASK);
    item->setText(QString("Flag: 0x%1")
            .arg(flag));
    flag = flag>>8; // Get the higher 8 bit of the flag

    childitem = new QStandardItem();
    if (SnifferUtil::getBit(flag,0))
    {
        childitem->setText("1... .... = Reserved bit: Set");
    }
    else
    {
        childitem->setText("0... .... = Reserved bit: Not set");
    }
    item->appendRow(childitem);

    childitem = new QStandardItem();
    if (SnifferUtil::getBit(flag,1))
    {
        childitem->setText(".1.. .... = Don't Fragment: Set");
    }
    else
    {
        childitem->setText(".0.. .... = Don't Fragment: Not set");
    }
    item->appendRow(childitem);

    childitem = new QStandardItem();
    if (SnifferUtil::getBit(flag,2))
    {
        childitem->setText("..1. .... = More Fragments: Set");
    }
    else
    {
        childitem->setText("..0. .... = More Fragments: Not set");
    }
    item->appendRow(childitem);



    ipitem->appendRow(item);
    // ip offset and time to live
    item = new QStandardItem();
    item->setText(QString("Fragment offset: %1").arg(ntohs(pip_header->ip_off)));
    ipitem->appendRow(item);
    item = new QStandardItem();
    item->setText(QString("Time to live: %1").arg(pip_header->ip_ttl));
    ipitem->appendRow(item);
    item = new QStandardItem();
    item->setText(QString("Header checksum: 0x%1").arg(ntohs(pip_header->ip_sum)));
    ipitem->appendRow(item);
    
    // ip Source and destionation
    item = new QStandardItem();
    item->setText(QString("Source: %1").arg(inet_ntoa(pip_header->ip_src)));
    ipitem->appendRow(item);
    item = new QStandardItem();
    item->setText(QString("Destination: %1").arg(inet_ntoa(pip_header->ip_dst)));
    ipitem->appendRow(item);
    
    int type = pip_header->ip_p;

    switch(type)
    {
        case IPPROTO_TCP:
            item = new QStandardItem();
            item->setText(QString("Type: TCP (0x%1)").arg(type,4, 16,QLatin1Char('0')));
            ipitem->appendRow(item);
            this->model->setItem(this->irow++, 0, ipitem);   

            addTcpNode(protoInd+1);
            break;
        case IPPROTO_UDP:
            item = new QStandardItem();
            item->setText(QString("Type: UDP (0x%1)").arg(type,4, 16,QLatin1Char('0')));
            ipitem->appendRow(item);
            this->model->setItem(this->irow++, 0, ipitem);   

            addUdpNode(protoInd+1);
            break;
        case IPPROTO_ICMP:
            item = new QStandardItem();
            item->setText(QString("Type: ICMP (0x%1)").arg(type,4, 16,QLatin1Char('0')));
            ipitem->appendRow(item);
            this->model->setItem(this->irow++, 0, ipitem);   

            addIcmpNode(protoInd+1);
            break;
        case IPPROTO_IGMP:
            item = new QStandardItem();
            item->setText(QString("Type: IGMP (0x%1)").arg(type,4, 16,QLatin1Char('0')));
            ipitem->appendRow(item);
            this->model->setItem(this->irow++, 0, ipitem);   

            addIgmpNode(protoInd+1);
            break;
        default:
            qDebug("Current the sniffer can not analysis the ethernet type:%0x", type);
            break;
    }
}

void AnalysisThread::addIpV6Node(int protoInd)
{
}

void AnalysisThread::addArpNode(int protoInd)
{
    struct ether_arp* parp_header = (struct ether_arp *)this->snifferData.protocalVec.at(protoInd)->pProtocal;

    QStandardItem* arpitem = new QStandardItem();
    unsigned short int op = ntohs(parp_header->ea_hdr.ar_op);
    if (op == 0x01)
    {
        arpitem->setText("Address Resolution Protocol (request)");
    } 
    else if (op == 0x02)
    {
        arpitem->setText("Address Resolution Protocol (response)");
    }

    // Hardware type, here we just analysis the Ether type
    QStandardItem* item = new QStandardItem();
    if (ntohs(parp_header->ea_hdr.ar_hrd) == 0x01)
    {
        item->setText("Hardware type: Ethernet (1)");
    }
    else
    {
        item->setText(QString("Hardware type: 1%").arg(ntohs(parp_header->ea_hdr.ar_hrd)));
    }
    arpitem->appendRow(item);
    // Protocal type, here we just analysis the ip type
    item = new QStandardItem();
    if (ntohs(parp_header->ea_hdr.ar_pro) == 0x0800)
    {
        item->setText("Protocol type: IP (0x0800)");
    }
    else
    {
        item->setText(QString("Protocol type: %1").arg(ntohs(parp_header->ea_hdr.ar_pro)));
    }
    arpitem->appendRow(item);

    // hardware size and protocal size
    item = new QStandardItem();
    item->setText(QString("Hardware size: %1").arg(parp_header->ea_hdr.ar_hln));
    arpitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Protocal size: %1").arg(parp_header->ea_hdr.ar_pln));
    arpitem->appendRow(item);

    item = new QStandardItem();
    if (op == 0x01)
    {
        item->setText("Opcode: request (1)");
    }
    else if (op == 0x02)
    {
        item->setText("Opcode: response (1)");
    }
    else
    {
        qDebug("Error arp op code: %0x", op);
    }
    arpitem->appendRow(item);

    // Add address
    item = new QStandardItem();
    item->setText(QString("Sender MAC address: %1")
            .arg(SnifferUtil::macToHost(parp_header->arp_sha)));
    arpitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Sender IP address: %1")
            .arg(SnifferUtil::netToIp(parp_header->arp_spa)));
    arpitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Target MAC address: %1")
            .arg(SnifferUtil::macToHost(parp_header->arp_tha)));
    arpitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Target IP address: %1")
            .arg(SnifferUtil::netToIp(parp_header->arp_tpa)));
    arpitem->appendRow(item);

    this->model->setItem(this->irow++, 0, arpitem);   
}

void AnalysisThread::addTcpNode(int protoInd)
{

}

void AnalysisThread::addUdpNode(int protoInd)
{

}

void AnalysisThread::addIcmpNode(int protoInd)
{

}

void AnalysisThread::addIgmpNode(int protoInd)
{

}

void AnalysisThread::run()
{
    this->exec();
}
