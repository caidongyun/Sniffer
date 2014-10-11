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
}

AnalysisThread::~AnalysisThread()
{

}

void AnalysisThread::anslysisData(const SnifferData& snifferData, int row)
{
    this->model->clear();

    // The Frame outline tree node
    this->model->setItem(0,0,this->addFrameNode(snifferData, row));
}

QStandardItem* AnalysisThread::addFrameNode(const SnifferData& snifferData, int row)
{
    QStandardItem *frameitem = new QStandardItem();
    frameitem->setText(QString("Frame %1: %2 bytes on wire (%3 bits),"
                "%2 bytes captured (%3 bits)"
                ).arg(row)
                 .arg(snifferData.iLength)
                 .arg(snifferData.iLength*8));

    QStandardItem *item = new QStandardItem();
    item->setText(QString("Encapsulation: %1").arg("Ethernet"));
    frameitem->appendRow(item);
    item = new QStandardItem();
    item->setText(QString("Arrival time: %1").arg(snifferData.strTime));
    frameitem->appendRow(item);
    item = new QStandardItem();
    item->setText(QString("Frame Number: %1").arg(row));
    frameitem->appendRow(item);
    item = new QStandardItem();
    item->setText(QString("Frame Length: %1 bytes (%2 bits)").
            arg(snifferData.iLength).arg(snifferData.iLength * 8));
    frameitem->appendRow(item);
    item = new QStandardItem();
    item->setText(QString("Protocal in frame: %1").arg(snifferData.strProtocol));
    frameitem->appendRow(item);

    return frameitem;
}

void AnalysisThread::run()
{
    this->exec();
}
