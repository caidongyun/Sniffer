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
    struct ether_header* pether_header = (struct ether_header *)this->snifferData.protocalVec.at(protoInd);
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
    item->setChild(0, 0, childitem);

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

    this->model->setItem(this->irow++, 0, etheritem); 


    // Get the protocal type
    int type = ntohs(pether_header->ether_type); 

    switch(type)
    {
        case ETHERTYPE_IP:
        {
            addIpNode(protoInd++);
            break;
        }
        case ETHERTYPE_IPV6:
        {
            addIpV6Node(protoInd++);
        }
        case ETHERTYPE_ARP:
        {
            addArpNode(protoInd++);
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
    //struct ip* pip_header = (struct ip *)this->;

}

void AnalysisThread::addIpV6Node(int protoInd)
{
    //SnifferProtocal* pProtocal = this->snifferData.protocalVec.at(protoInd);
}

void AnalysisThread::addArpNode(int protoInd)
{

}


void AnalysisThread::run()
{
    this->exec();
}
