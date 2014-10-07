/*
 * =====================================================================================
 *
 *       Filename:  snifferthread.h
 *
 *    Description:  the thread which sniffer the net packet and process the packet into
 *                  normal data structure.
 *
 *        Version:  1.0
 *        Created:  2014年09月22日 10时16分42秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */

#ifndef SNIFFERTHREAD_H_
#define SNIFFERTHREAD_H_

#include <QtCore/QThread>
#include <QStandardItemModel>

#include <time.h>

#include "sniffer.h"
#include "sniffertypes.h"
#include "snifferutil.h"
#include "mytableview.h"

class QStandardItemModel;
class MyTableView;
class QThread;

class SnifferThread : public QThread
{
    Q_OBJECT

    signals:
        void addPacketItem(QString strTime, QString strSrc,
                QString strDst, QString strProtocol,int iLength,
                QString info);
    public:
        SnifferThread();
        SnifferThread(Sniffer* pSniffer, MyTableView*  tableview);
        ~SnifferThread();
        void run();
        void stopSniffer();
        void startSniffer();

    private:
        Sniffer *pSniffer;
        MyTableView* tableview;
        volatile bool bStop;
};
#endif
