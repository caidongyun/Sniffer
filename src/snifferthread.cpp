/*
 * =====================================================================================
 *
 *       Filename:  snifferthread.cpp
 *
 *    Description:  the thread which sniffer the net packet and process the packet into
 *                  normal data structure.
 *
 *        Version:  1.0
 *        Created:  2014年09月22日 10时23分42秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#include "../include/snifferthread.h"

SnifferThread::SnifferThread()
{
    this->pSniffer = NULL;
    this->bStop = true;
}

SnifferThread::SnifferThread(Sniffer *pSniffer, MyTableView* tableview)
{
    this->pSniffer = pSniffer;
    this->bStop = true;
    this->tableview = tableview;
}

SnifferThread::~SnifferThread()
{
    this->quit();
    this->wait();
}

void SnifferThread::run()
{
    int res = -1;
    bStop = false;
    QByteArray rawData;
    SnifferData tmpSnifferData;
    SnifferProtocal* snifferProtocal = NULL;
    int iLength = 0;
    QString strTime;
    QString strSrc;
    QString strDst;
    QString info;
    QString strProtocol;
    while (bStop != true && (res = this->pSniffer->captureOnce()) >= 0)
    {
        int tmpFrameOffset = 0; // The offset of the frame
        
        // Init the tmp
        iLength = 0;
        strTime = "";
        strSrc = "";
        strDst = "";
        info = "";
        strProtocol = "";

        tmpSnifferData.reInit();
        if (res == 0)
        {
            continue;
        }

        rawData.clear();
        rawData.setRawData((const char*)pSniffer->packetData, pSniffer->header->caplen);

        tmpSnifferData.rawData = rawData;
        
        void* pheader = new uint8_t[sizeof(struct pcap_pkthdr)];
        tmpSnifferData.header = (struct pcap_pkthdr*)pheader;
        memcpy(pheader, pSniffer->header, sizeof(*pSniffer->header));

        iLength = pSniffer->header->caplen;
        strTime = SnifferUtil::getFormatTime(&pSniffer->header->ts.tv_sec);
        strProtocol = "Unknown"; 

        const u_char* pPktData = pSniffer->packetData;

        // Set the ether header
        struct ether_header *tmpEthHeader = (struct ether_header*)(pPktData + tmpFrameOffset);
        tmpFrameOffset += sizeof(struct ether_header);

        snifferProtocal = new SnifferProtocal();
        snifferProtocal->strProtocal = SnifferType::ETHER_PROTOCAL;
        snifferProtocal->pProtocal = SnifferUtil::mallocMem(tmpEthHeader, sizeof(struct ether_header));
        tmpSnifferData.protocalVec.append(snifferProtocal);

        // Set the src and dst addr
        strSrc = SnifferUtil::macToHost(tmpEthHeader->ether_shost);
        strDst = SnifferUtil::macToHost(tmpEthHeader->ether_dhost);

        int tmpProtocal = ntohs(tmpEthHeader->ether_type);

        bool canResolve = true;
        switch (tmpProtocal)
        {
            case ETHERTYPE_IP:
            case ETHERTYPE_IPV6:
            {
                int ipProtocal = -1;
                if (tmpProtocal == ETHERTYPE_IP)
                {
                    //qDebug("Resolve ipv4 header:%x" ,tmpProtocal);
                    struct ip* tmpIpHeader = (struct ip*)(pPktData + tmpFrameOffset); // Add the offset of mac header
                    tmpFrameOffset += sizeof(struct ip);
                    snifferProtocal = new SnifferProtocal();
                    snifferProtocal->strProtocal = SnifferType::IP_PROTOCAL;
                    snifferProtocal->pProtocal = SnifferUtil::mallocMem(tmpIpHeader, sizeof(struct ip));
                    tmpSnifferData.protocalVec.append(snifferProtocal);


                    strSrc = inet_ntoa(tmpIpHeader->ip_src);
                    strDst = inet_ntoa(tmpIpHeader->ip_dst);
                    
                    //qDebug("Src:%s--Des:%s", tmpSnifferData.strSrc.toStdString().data(), 
                    //        tmpSnifferData.strDst.toStdString().data());

                    ipProtocal = tmpIpHeader->ip_p;
                } 
                else
                {
                    //qDebug("Resolve ipv6 header:%x" ,tmpProtocal);
                    struct ip6_hdr* tmpIpHeader = (struct ip6_hdr*)(pPktData + tmpFrameOffset);
                    tmpFrameOffset += sizeof(struct ip6_hdr);
                    snifferProtocal = new SnifferProtocal();
                    snifferProtocal->strProtocal = SnifferType::IPV6_PROTOCAL;
                    snifferProtocal->pProtocal = SnifferUtil::mallocMem(tmpIpHeader, sizeof(struct ip6_hdr));
                    tmpSnifferData.protocalVec.append(snifferProtocal);

                    //char buf[INET6_ADDRSTRLEN];
                    //inet_ntop(AF_INET6,(void *)&tmpIpHeader->ip6_src,buf, INET6_ADDRSTRLEN);
                    ipProtocal = tmpIpHeader->ip6_ctlun.ip6_un1.ip6_un1_nxt;

                    strSrc = SnifferUtil::ipV6ToHost(tmpIpHeader->ip6_src);
                    strDst = SnifferUtil::ipV6ToHost(tmpIpHeader->ip6_dst);

                    //qDebug("Src:%s--Des:%s", tmpSnifferData.strSrc.toStdString().data(), 
                    //        tmpSnifferData.strDst.toStdString().data());
                }
                switch (ipProtocal)
                {
                    case IPPROTO_TCP:
                    {
                        struct tcphdr *tmpTcpHeader = (struct tcphdr*)(pPktData + tmpFrameOffset);
                        tmpFrameOffset += sizeof(struct tcphdr);

                        strProtocol = "TCP";
                        int tcpProtocal = ntohs(tmpTcpHeader->source);
                        switch (tcpProtocal)
                        {
                            case FTP_PORT:
                            {
                                strProtocol = "FTP";
                                break;
                            }
                            case TELNET_PORT:
                            {
                                strProtocol = "TELNET";
                                break;
                            }
                            case SMTP_PORT:
                            {
                                strProtocol = "SMTP";
                                break;
                            }
                            case HTTP_PORT:
                            {
                                strProtocol = "HTTP";
                                break;
                            }
                            case HTTPS_PORT:
                            {
                                strProtocol = "HTTPS";
                                break;
                            }
                            case POP3_PORT:
                            {
                                strProtocol = "POP3";
                                break;
                            }
                            default:
                            {
                                break;
                            }
                        }

                        break;
                    }
                    case IPPROTO_UDP:
                    {
                        struct udphdr *tmpUdpHeader = (struct udphdr*)(pPktData + tmpFrameOffset);
                        tmpFrameOffset += sizeof(struct udphdr);

                        strProtocol = "UDP";
                        int udpProtocal = ntohs(tmpUdpHeader->source);
                        switch (udpProtocal)
                        {
                            case DNS_PORT:
                            {
                                strProtocol = "DNS";
                                break;
                            }
                            case SNMP_PORT:
                            {
                                strProtocol = "TCP";
                                break;
                            }
                            default:
                            {
                                break;
                            }
                        }

                        break;
                    }
                    case IPPROTO_ICMP:
                    {
                        strProtocol = "ICMP";
                        break;
                    }
                    case IPPROTO_ICMPV6:
                    {
                        strProtocol = "ICMPV6";
                        break;
                    }
                    case IPPROTO_IGMP:
                    {
                        strProtocol = "IGMP";
                        break;
                    }
                    default:
                        //qDebug("We do not resolve the ip protocal: %0x", ipProtocal);
                        break;
                }
                break;
            }
            case ETHERTYPE_ARP:
            {
                //qDebug("Resolve arp header");
                struct ether_arp *tmpArpHeader = (struct ether_arp*)(pPktData + tmpFrameOffset);
                tmpFrameOffset += sizeof(struct ether_arp);

                snifferProtocal = new SnifferProtocal();
                snifferProtocal->strProtocal = SnifferType::ARP_PROTOCAL;
                snifferProtocal->pProtocal = SnifferUtil::mallocMem(tmpArpHeader, sizeof(struct ether_arp));
                tmpSnifferData.protocalVec.append(snifferProtocal);
                
                strProtocol = "ARP";

                unsigned short int op = ntohs(tmpArpHeader->ea_hdr.ar_op);
                if (op == 0x1)
                {
                    // arp Request 
                    if (SnifferUtil::netToIp(tmpArpHeader->arp_spa) == 
                            SnifferUtil::netToIp(tmpArpHeader->arp_tpa))
                    {
                        info = "Gratuitous ARP for " + SnifferUtil::netToIp(tmpArpHeader->arp_spa)
                            + "(Request)";
                    }
                    else
                    {
                        info = "Who has " + SnifferUtil::netToIp(tmpArpHeader->arp_tpa) +
                            "? Tell:" + SnifferUtil::netToIp(tmpArpHeader->arp_spa);
                    }
                }
                else if (op == 0x2)
                {
                    // arp reply
                }
                else
                {
                    info = "error op code " + QString::number(op,16);
                }
                break;
            }
            default:
            {
                canResolve = false;
                qDebug("Current the sniffer can not analysis the ethernet type:%0x", tmpProtocal);
                break;
            }

        }
        if (canResolve)
        {
            pSniffer->snifferDataVector.append(tmpSnifferData);
            
            emit addPacketItem(strTime, strSrc,
                    strDst, strProtocol,
                    iLength, info);
        }

    }

}

void SnifferThread::startSniffer()
{
    this->bStop = false;
}

void SnifferThread::stopSniffer()
{
    this->bStop = true;

}
