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
    char timestr[16];
    QByteArray rawData;
    SnifferData tmpSnifferData;
    while (bStop != true && (res = this->pSniffer->captureOnce()) >= 0)
    {
        int tmpFrameOffset = 0; // The offset of the frame
        
        tmpSnifferData.reInit();
        if (res == 0)
        {
            continue;
        }

        rawData.clear();
        rawData.setRawData((const char*)pSniffer->packetData, pSniffer->header->caplen);
        strftime(timestr, sizeof(timestr), "%H:%M:%S", localtime(&pSniffer->header->ts.tv_sec));

        tmpSnifferData.rawData = rawData;
        tmpSnifferData.iLength = pSniffer->header->caplen;
        tmpSnifferData.strTime = timestr;
        tmpSnifferData.strProtocol = "Unknown"; 
        const u_char* pPktData = pSniffer->packetData;

        // Set the ether header
        struct ether_header *tmpEthHeader = (struct ether_header*)(pPktData + tmpFrameOffset);
        tmpFrameOffset += sizeof(struct ether_header);

        // Set the src and dst addr
        tmpSnifferData.strSrc = SnifferUtil::macToHost(tmpEthHeader->ether_shost);
        tmpSnifferData.strDst = SnifferUtil::macToHost(tmpEthHeader->ether_dhost);

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

                    tmpSnifferData.strSrc = inet_ntoa(tmpIpHeader->ip_src);
                    tmpSnifferData.strDst = inet_ntoa(tmpIpHeader->ip_dst);
                    
                    //qDebug("Src:%s--Des:%s", tmpSnifferData.strSrc.toStdString().data(), 
                    //        tmpSnifferData.strDst.toStdString().data());

                    ipProtocal = tmpIpHeader->ip_p;
                } 
                else
                {
                    //qDebug("Resolve ipv6 header:%x" ,tmpProtocal);
                    struct ip6_hdr* tmpIpHeader = (struct ip6_hdr*)(pPktData + tmpFrameOffset);
                    tmpFrameOffset += sizeof(struct ip6_hdr);

                    //char buf[INET6_ADDRSTRLEN];
                    //inet_ntop(AF_INET6,(void *)&tmpIpHeader->ip6_src,buf, INET6_ADDRSTRLEN);
                    ipProtocal = tmpIpHeader->ip6_ctlun.ip6_un1.ip6_un1_nxt;

                    tmpSnifferData.strSrc = SnifferUtil::ipV6ToHost(tmpIpHeader->ip6_src);
                    tmpSnifferData.strDst = SnifferUtil::ipV6ToHost(tmpIpHeader->ip6_dst);

                    //qDebug("Src:%s--Des:%s", tmpSnifferData.strSrc.toStdString().data(), 
                    //        tmpSnifferData.strDst.toStdString().data());
                }
                switch (ipProtocal)
                {
                    case IPPROTO_TCP:
                    {
                        struct tcphdr *tmpTcpHeader = (struct tcphdr*)(pPktData + tmpFrameOffset);
                        tmpFrameOffset += sizeof(struct tcphdr);

                        tmpSnifferData.strProtocol = "TCP";
                        int tcpProtocal = ntohs(tmpTcpHeader->source);
                        switch (tcpProtocal)
                        {
                            case FTP_PORT:
                            {
                                tmpSnifferData.strProtocol = "FTP";
                                break;
                            }
                            case TELNET_PORT:
                            {
                                tmpSnifferData.strProtocol = "TELNET";
                                break;
                            }
                            case SMTP_PORT:
                            {
                                tmpSnifferData.strProtocol = "SMTP";
                                break;
                            }
                            case HTTP_PORT:
                            {
                                tmpSnifferData.strProtocol = "HTTP";
                                break;
                            }
                            case HTTPS_PORT:
                            {
                                tmpSnifferData.strProtocol = "HTTPS";
                                break;
                            }
                            case POP3_PORT:
                            {
                                tmpSnifferData.strProtocol = "POP3";
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

                        tmpSnifferData.strProtocol = "UDP";
                        int udpProtocal = ntohs(tmpUdpHeader->source);
                        switch (udpProtocal)
                        {
                            case DNS_PORT:
                            {
                                tmpSnifferData.strProtocol = "DNS";
                                break;
                            }
                            case SNMP_PORT:
                            {
                                tmpSnifferData.strProtocol = "TCP";
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
                        tmpSnifferData.strProtocol = "ICMP";
                        break;
                    }
                    case IPPROTO_ICMPV6:
                    {
                        tmpSnifferData.strProtocol = "ICMPV6";
                        break;
                    }
                    case IPPROTO_IGMP:
                    {
                        tmpSnifferData.strProtocol = "IGMP";
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
                struct ether_arp *arpHeader = (struct ether_arp*)(pPktData + tmpFrameOffset);
                tmpFrameOffset += sizeof(struct ether_arp);
                
                tmpSnifferData.strProtocol = "ARP";

                unsigned short int op = ntohs(arpHeader->ea_hdr.ar_op);
                if (op == 0x1)
                {
                    // arp Request 
                    tmpSnifferData.info = "Gratuitous ARP for " + SnifferUtil::netToIp(arpHeader->arp_spa)
                        + "(Request)";
                }
                else if (op == 0x2)
                {
                    // arp reply
                    tmpSnifferData.info = "Who has " + SnifferUtil::netToIp(arpHeader->arp_spa) +
                        "? Tell" + SnifferUtil::netToIp(arpHeader->arp_tpa);
                }
                else
                {
                    tmpSnifferData.info = "error op code " + QString::number(op,16);
                }
                break;
            }
            default:
            {
                canResolve = false;
                qDebug("We do not resolve the mac type: %0x", tmpProtocal);
                break;
            }

        }
        if (canResolve)
        {
            pSniffer->snifferDataVector.append(tmpSnifferData);
            emit addPacketItem(tmpSnifferData.strTime, tmpSnifferData.strSrc,
                    tmpSnifferData.strDst, tmpSnifferData.strProtocol,
                    tmpSnifferData.iLength, tmpSnifferData.info);
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
