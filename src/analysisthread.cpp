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
    this->offset = 0;
    this->len = 0;

    this->snifferData = snifferData;
    addFrameNode(frameIndex);
}

void AnalysisThread::setUserRoleData(QStandardItem* item)
{
    item->setData(this->offset, ROLE_OFFSET);
    item->setData(this->len, ROLE_LEN);
}

void AnalysisThread::addFrameNode(int frameIndex)
{
    this->len = snifferData.rawData.size();
    QStandardItem *frameitem = new QStandardItem();
    frameitem->setText(QString("Frame %1: %2 bytes on wire (%3 bits),"
                "%4 bytes captured (%5 bits)"
                ).arg(frameIndex)
                 .arg(snifferData.header->len)
                 .arg(snifferData.header->len * 8)
                 .arg(snifferData.header->caplen)
                 .arg(snifferData.header->caplen * 8));
    setUserRoleData(frameitem);


    QStandardItem *item = new QStandardItem();
    item->setText(QString("Encapsulation: %1").arg("Ethernet"));
    setUserRoleData(item);
    frameitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Arrival time: %1 CST").
            arg(SnifferUtil::getFormatTimeCST(&snifferData.header->ts.tv_sec)));
    setUserRoleData(item);
    frameitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Frame Number: %1").arg(frameIndex));
    setUserRoleData(item);
    frameitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Frame Length: %1 bytes (%2 bits)").
            arg(snifferData.header->len).arg(snifferData.header->len * 8));
    setUserRoleData(item);
    frameitem->appendRow(item);

    item = new QStandardItem();
    item->setText(QString("Capture Length: %1 bytes (%2 bits)").
            arg(snifferData.header->caplen).arg(snifferData.header->caplen * 8));
    setUserRoleData(item);
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
    this->offset = 0;
    this->len = sizeof(struct ether_header);
    setUserRoleData(etheritem);
    
    QString tmpmacDst = SnifferUtil::macToHost(pether_header->ether_dhost);
    QString tmpmacSrc = SnifferUtil::macToHost(pether_header->ether_shost);
    etheritem->setText(QString("Ethernet V2, Src: %1, Dst: %2")
            .arg(tmpmacSrc)
            .arg(tmpmacDst));


    // Destination
    QStandardItem* item = new QStandardItem();
    item->setText("Destination: " + tmpmacDst);
    this->len = sizeof(pether_header->ether_dhost);
    setUserRoleData(item);
    
    QStandardItem* childitem = new QStandardItem();
    this->len = sizeof(pether_header->ether_dhost);
    setUserRoleData(childitem);
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
    this->len = 3;
    setUserRoleData(childitem);
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

    setUserRoleData(childitem);
    item->appendRow(childitem);
    etheritem->appendRow(item);
    this->offset += sizeof(pether_header->ether_dhost);

    // Source 
    item = new QStandardItem();
    item->setText("Source: " + tmpmacSrc);
    this->len = sizeof(pether_header->ether_shost);
    setUserRoleData(item);
    
    childitem = new QStandardItem();
    this->len = sizeof(pether_header->ether_shost);
    setUserRoleData(childitem);
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
    this->len = 3;
    setUserRoleData(childitem);
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

    setUserRoleData(childitem);
    item->appendRow(childitem);

    etheritem->appendRow(item);
    this->offset += sizeof(pether_header->ether_shost);

    // Get the protocal type
    int type = ntohs(pether_header->ether_type);
    this->len = sizeof(pether_header->ether_type);

    switch(type)
    {
        case ETHERTYPE_IP:
        {
            item = new QStandardItem();
            item->setText(QString("Type: IP (0x%1)").arg(type,4, 16,QLatin1Char('0')));
            setUserRoleData(item);
            etheritem->appendRow(item);
            this->offset += this->len;
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
            setUserRoleData(item);
            this->offset += this->len;

            addIpV6Node(protoInd+1);
            break;
        }
        case ETHERTYPE_ARP:
        {
            item = new QStandardItem();
            item->setText(QString("Type: ARP (0x%1)").arg(type,4, 16,QLatin1Char('0')));
            setUserRoleData(item);
            etheritem->appendRow(item);
            this->model->setItem(this->irow++, 0, etheritem);   
            this->offset += this->len;

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
    this->len = sizeof(struct ip);
    setUserRoleData(ipitem);
    ipitem->setText(QString("Internet Protocol Version 4, Src: %1, Dst: %2")
            .arg(inet_ntoa(pip_header->ip_src))
            .arg(inet_ntoa(pip_header->ip_dst)));

    QStandardItem* item = new QStandardItem();
    item->setText("Version: 4");
    this->len = 1;
    setUserRoleData(item);
    ipitem->appendRow(item);

    item = new QStandardItem();
    this->len = 1;
    item->setText(QString("Header length: %1 bytes").arg(pip_header->ip_hl * 4));
    setUserRoleData(item);
    ipitem->appendRow(item);
    this->offset += this->len;

    // ECN(last 2 bits) and DSCP(first 6 bits) type
    item = new QStandardItem();
    this->len = sizeof(pip_header->ip_tos);
    item->setText(QString("Differentiated Services Field: 0x%1 (DSCP 0x%2: %3; ECN: 0x%4: %5)")
            .arg(SnifferUtil::byteToHexStr(pip_header->ip_tos,16))
            .arg(SnifferUtil::byteToHexStr(IPTOS_DSCP(pip_header->ip_tos),16))
            .arg(SnifferUtil::getDSCPDesc(pip_header->ip_tos))
            .arg(SnifferUtil::byteToHexStr(IPTOS_ECN(pip_header->ip_tos),16))
            .arg(SnifferUtil::getECNDesc(pip_header->ip_tos)));
    setUserRoleData(item);

    // DSCP
    QStandardItem* childitem = new QStandardItem();
    childitem->setText(QString("%1.. = Differentiated Services Codepoint: %2 (0x%3)")
            .arg(SnifferUtil::byteToHexStr(pip_header->ip_tos,2).left(6))
            .arg(SnifferUtil::getDSCPDesc(pip_header->ip_tos))
            .arg(SnifferUtil::byteToHexStr(IPTOS_DSCP(pip_header->ip_tos),16)));
    setUserRoleData(childitem);
    item->appendRow(childitem);

    // ECN
    childitem = new QStandardItem();
    childitem->setText(QString(".... ..%1 = Explicit Congestion Notification: %2 (0x%3)")
            .arg(SnifferUtil::byteToHexStr(pip_header->ip_tos,2).right(2))
            .arg(SnifferUtil::getECNDesc(pip_header->ip_tos))
            .arg(SnifferUtil::byteToHexStr(IPTOS_ECN(pip_header->ip_tos),16)));
    setUserRoleData(childitem);
    item->appendRow(childitem);
    
    ipitem->appendRow(item);
    this->offset += this->len;

    // Total length and Indentification
    item = new QStandardItem();
    this->len = sizeof(pip_header->ip_len);
    item->setText(QString("Total Length: %1").arg(ntohs(pip_header->ip_len)));
    setUserRoleData(item);
    ipitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    this->len = sizeof(pip_header->ip_id);
    item->setText(QString("Identification: 0x%1 (%2)")
            .arg(ntohs(pip_header->ip_id),0,16)
            .arg(ntohs(pip_header->ip_id)));
    setUserRoleData(item);
    ipitem->appendRow(item);
    this->offset += this->len;

    // ip Flag
    item = new QStandardItem();
    this->len = sizeof(pip_header->ip_off);
    short flag = pip_header->ip_off & (0xffff - IP_OFFMASK);
    item->setText(QString("Flag: 0x%1")
            .arg(flag));
    flag = flag>>8; // Get the higher 8 bit of the flag
    setUserRoleData(item);

    childitem = new QStandardItem();
    if (SnifferUtil::getBit(flag,0))
    {
        childitem->setText("1... .... = Reserved bit: Set");
    }
    else
    {
        childitem->setText("0... .... = Reserved bit: Not set");
    }
    setUserRoleData(childitem);
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
    setUserRoleData(childitem);
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
    setUserRoleData(childitem);
    item->appendRow(childitem);

    ipitem->appendRow(item);
    // ip offset and time to live
    item = new QStandardItem();
    this->len = sizeof(pip_header->ip_off);
    item->setText(QString("Fragment offset: %1").arg(ntohs(pip_header->ip_off)));
    setUserRoleData(item);
    ipitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    this->len = sizeof(pip_header->ip_ttl);
    item->setText(QString("Time to live: %1").arg(pip_header->ip_ttl));
    setUserRoleData(item);
    ipitem->appendRow(item);
    this->offset += this->len;


    QStandardItem* protoitem = new QStandardItem();
    this->len = sizeof(pip_header->ip_p);
    setUserRoleData(protoitem);
    ipitem->appendRow(protoitem);
    this->offset += this->len;

    item = new QStandardItem();
    this->len = sizeof(pip_header->ip_sum);
    item->setText(QString("Header checksum: 0x%1").arg(ntohs(pip_header->ip_sum),0,16));
    setUserRoleData(item);
    ipitem->appendRow(item);
    this->offset += this->len;
    
    // ip Source and destionation
    item = new QStandardItem();
    this->len = sizeof(pip_header->ip_src);
    item->setText(QString("Source: %1").arg(inet_ntoa(pip_header->ip_src)));
    setUserRoleData(item);
    ipitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    this->len = sizeof(pip_header->ip_dst);
    item->setText(QString("Destination: %1").arg(inet_ntoa(pip_header->ip_dst)));
    setUserRoleData(item);
    ipitem->appendRow(item);
    this->offset += this->len;
    
    int type = pip_header->ip_p;

    switch(type)
    {
        case IPPROTO_TCP:
            protoitem->setText(QString("Type: TCP (0x%1)").arg(type,2,16,QLatin1Char('0')));
            this->model->setItem(this->irow++, 0, ipitem);   

            addTcpNode(protoInd+1);
            break;
        case IPPROTO_UDP:
            protoitem->setText(QString("Type: UDP (0x%1)").arg(type,2, 16,QLatin1Char('0')));
            this->model->setItem(this->irow++, 0, ipitem);   

            addUdpNode(protoInd+1);
            break;
        case IPPROTO_ICMP:
            protoitem->setText(QString("Type: ICMP (0x%1)").arg(type,2, 16,QLatin1Char('0')));
            this->model->setItem(this->irow++, 0, ipitem);   

            addIcmpNode(protoInd+1);
            break;
        case IPPROTO_IGMP:
            protoitem->setText(QString("Type: IGMP (0x%1)").arg(type,2, 16,QLatin1Char('0')));
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
    this->len = sizeof(struct ether_arp);
    unsigned short int op = ntohs(parp_header->ea_hdr.ar_op);
    if (op == 0x01)
    {
        arpitem->setText("Address Resolution Protocol (request)");
    } 
    else if (op == 0x02)
    {
        arpitem->setText("Address Resolution Protocol (response)");
    }
    setUserRoleData(arpitem);

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
    this->len = sizeof(parp_header->ea_hdr.ar_hrd);
    setUserRoleData(item);
    arpitem->appendRow(item);
    this->offset += this->len;
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
    this->len = sizeof(parp_header->ea_hdr.ar_pro);
    setUserRoleData(item);
    arpitem->appendRow(item);
    this->offset += this->len;

    // hardware size and protocal size
    item = new QStandardItem();
    this->len = sizeof(parp_header->ea_hdr.ar_hln);
    item->setText(QString("Hardware size: %1").arg(parp_header->ea_hdr.ar_hln));
    setUserRoleData(item);
    arpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    this->len = sizeof(parp_header->ea_hdr.ar_pln);
    item->setText(QString("Protocal size: %1").arg(parp_header->ea_hdr.ar_pln));
    setUserRoleData(item);
    arpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    this->len = sizeof(op);
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
    setUserRoleData(item);
    arpitem->appendRow(item);
    this->offset += this->len;

    // Add address
    item = new QStandardItem();
    this->len = sizeof(parp_header->arp_sha);
    item->setText(QString("Sender MAC address: %1")
            .arg(SnifferUtil::macToHost(parp_header->arp_sha)));
    setUserRoleData(item);
    arpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    this->len = sizeof(parp_header->arp_spa);
    item->setText(QString("Sender IP address: %1")
            .arg(SnifferUtil::netToIp(parp_header->arp_spa)));
    setUserRoleData(item);
    arpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    this->len = sizeof(parp_header->arp_tha);
    item->setText(QString("Target MAC address: %1")
            .arg(SnifferUtil::macToHost(parp_header->arp_tha)));
    setUserRoleData(item);
    arpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    this->len = sizeof(parp_header->arp_tpa);
    item->setText(QString("Target IP address: %1")
            .arg(SnifferUtil::netToIp(parp_header->arp_tpa)));
    setUserRoleData(item);
    arpitem->appendRow(item);
    this->offset += this->len;

    this->model->setItem(this->irow++, 0, arpitem);   
}

void AnalysisThread::addTcpNode(int protoInd)
{
    struct tcphdr* ptcp_header = (struct tcphdr *)this->snifferData.protocalVec.at(protoInd)->pProtocal;

    QStandardItem* tcpitem = new QStandardItem();
    this->len = sizeof(struct tcphdr);
    setUserRoleData(tcpitem);

    QStandardItem* item = new QStandardItem();
    u_int16_t src_port = ntohs(ptcp_header->source);
    this->len = sizeof(src_port);
    item->setText(QString("Source port: %1").arg(src_port));
    setUserRoleData(item);
    tcpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    u_int16_t dst_port = ntohs(ptcp_header->dest);
    this->len = sizeof(dst_port);
    item->setText(QString("Destination port: %1").arg(dst_port));
    setUserRoleData(item);
    tcpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    u_int32_t seq = ntohl(ptcp_header->seq);
    this->len = sizeof(seq);
    item->setText(QString("Sequence number: %1    (absolute sequence number)")
            .arg(seq));
    setUserRoleData(item);
    tcpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    u_int32_t ack_seq = ntohl(ptcp_header->ack_seq);
    this->len = sizeof(ack_seq);
    item->setText(QString("Acknowledgment number: %1    (absolute ack number)")
            .arg(ack_seq));
    setUserRoleData(item);
    tcpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    this->len = 1; // The header length
    item->setText(QString("Header length: %1 bytes")
            .arg(ptcp_header->doff * 4)); // The four bit multi 4 is the bytes
    setUserRoleData(item);
    tcpitem->appendRow(item);

    // The flag
    item = new QStandardItem();
    this->len = 2;
    setUserRoleData(item);
    QStringList flag; // The flag such as ack fin

    QStandardItem* childitem = new QStandardItem();
    this->len = 1;
    setUserRoleData(childitem);
    childitem->setText(QString("0000 00.. .... = Reserved: Not set"));
    item->appendRow(childitem);
    this->offset += this->len;

    childitem = new QStandardItem();
    setUserRoleData(childitem);
    if (ptcp_header->urg)
    {
        childitem->setText(".... ..1. .... = Urgent: Set");
        flag << "urg";
    }
    else
    {
        childitem->setText(".... ..0. .... = Urgent: Set");
    }
    item->appendRow(childitem);

    childitem = new QStandardItem();
    setUserRoleData(childitem);
    if (ptcp_header->ack)
    {
        childitem->setText(".... ...1 .... = Acknowledgment: Set");
        flag << "ack";
    }
    else
    {
        childitem->setText(".... ...0 .... = Acknowledgment: Not set");
    }
    item->appendRow(childitem);

    childitem = new QStandardItem();
    setUserRoleData(childitem);
    if (ptcp_header->psh)
    {
        childitem->setText(".... .... 1... = Push: Set");
        flag << "push";
    }
    else
    {
        childitem->setText(".... .... 0... = Push: Not set");
    }
    item->appendRow(childitem);

    childitem = new QStandardItem();
    setUserRoleData(childitem);
    if (ptcp_header->rst)
    {
        childitem->setText(".... .... .1.. = Reset: Set");
        flag << "reset";
    }
    else
    {
        childitem->setText(".... .... .0.. = Reset: Not set");
    }
    item->appendRow(childitem);

    childitem = new QStandardItem();
    setUserRoleData(childitem);
    if (ptcp_header->syn)
    {
        childitem->setText(".... .... .1.. = Reset: Set");
        flag << "syn";
    }
    else
    {
        childitem->setText(".... .... .0.. = Reset: Not set");
    }
    item->appendRow(childitem);

    childitem = new QStandardItem();
    setUserRoleData(childitem);
    if (ptcp_header->fin)
    {
        childitem->setText(".... .... ...1 = Fin: Set");
        flag << "fin";
    }
    else
    {
        childitem->setText(".... .... ...0 = Fin: Not set");
    }
    item->appendRow(childitem);
    
    item->setText(QString("Flags: %1")
            .arg(flag.join("/")));

    tcpitem->appendRow(item);
    this->offset += this->len;
    // The flag end
    
    item = new QStandardItem();
    u_int16_t window = ntohs(ptcp_header->window);
    this->len = sizeof(window);
    item->setText(QString("Window size value: %1").arg(window));
    setUserRoleData(item);
    tcpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    this->len = sizeof(ptcp_header->check);
    item->setText(QString("Checksum: 0x%1 [validation disabled]")
            .arg(ntohs(ptcp_header->check),4, 16));
    setUserRoleData(item);
    tcpitem->appendRow(item);
    this->offset += this->len;

    tcpitem->setText(QString("Transmission Control Protocol, Src Port: %1, Dst Port: %2, Seq: %3, Ack: %4")
            .arg(src_port)
            .arg(dst_port)
            .arg(seq)
            .arg(ack_seq));
    this->model->setItem(this->irow++, 0, tcpitem);   
}

void AnalysisThread::addUdpNode(int protoInd)
{
    struct udphdr* pudp_header = (struct udphdr *)this->snifferData.protocalVec.at(protoInd)->pProtocal;

    QStandardItem* udpitem = new QStandardItem();
    this->len = sizeof(struct udphdr);
    setUserRoleData(udpitem);

    QStandardItem* item = new QStandardItem();
    uint16_t src_port = ntohs(pudp_header->source);
    this->len = sizeof(src_port);
    item->setText(QString("Source port: %1").arg(src_port));
    setUserRoleData(item);
    udpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    uint16_t dst_port = ntohs(pudp_header->dest);
    this->len = sizeof(dst_port);
    item->setText(QString("Destination port: %1").arg(dst_port));
    setUserRoleData(item);
    udpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    u_int16_t len = ntohs(pudp_header->len);
    this->len = sizeof(len);
    item->setText(QString("Length: %1").arg(len));
    setUserRoleData(item);
    udpitem->appendRow(item);
    this->offset += this->len;

    item = new QStandardItem();
    u_int16_t check = ntohs(pudp_header->check);
    this->len = sizeof(check);
    item->setText(QString("Checksum: 0x%1 [validation disabled]")
            .arg(check,0,16));
    setUserRoleData(item);
    udpitem->appendRow(item);
    this->offset += this->len;

    udpitem->setText(QString("User Datagram Protocol, Src Port: %1, Dst Port: %2")
            .arg(src_port)
            .arg(dst_port));

    this->model->setItem(this->irow++, 0, udpitem);   
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
