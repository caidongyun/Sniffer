/*
 * =====================================================================================
 *
 *       Filename:  analysisthread.h
 *
 *    Description:  Analysis the thread, and update the ui
 *
 *        Version:  1.0
 *        Created:  2014年10月11日 19时39分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#ifndef ANALYSISTHREAD_H_
#define ANALYSISTHREAD_H_

#include <QtCore/QThread>
#include <QStandardItemModel>

#include "sniffertypes.h"
#include "prototree.h"
#include "snifferutil.h"

#define ROLE_OFFSET Qt::UserRole + 1
#define ROLE_LEN Qt::UserRole + 2

class AnalysisThread : public QThread
{
    Q_OBJECT

    public:
        AnalysisThread(QStandardItemModel* model);
        AnalysisThread();
        ~AnalysisThread();
        void run();
        void anslysisData(const SnifferData& snifferData, int frameIndex);

    private:
        // Add role value to the node
        void setUserRoleData(QStandardItem* item);

        // Add the frame outline node in the prototree
        void addFrameNode(int frameIndex);

        // Add the ether node
        void addEtherNode(int protoInd);

        // Add ip node
        void addIpNode(int protoInd);

        // Add ipv6 node
        void addIpV6Node(int protoInd);

        // Add arp node
        void addArpNode(int protoInd);

        // Add tcp node
        void addTcpNode(int protoInd);

        // Add udp Node
        void addUdpNode(int protoInd);

        // Add Icmp node
        void addIcmpNode(int protoInd);

        // Add igmp node
        void addIgmpNode(int protoInd);

        QStandardItemModel* model;
        SnifferData snifferData;
        int irow;     // The row index of current processed node

        int offset;   // The offset of current processed node
        int len;      // The len of current processed node
};

#endif
