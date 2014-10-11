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


class AnalysisThread : public QThread
{
    Q_OBJECT

    public:
        AnalysisThread(QStandardItemModel* model);
        AnalysisThread();
        ~AnalysisThread();
        void run();
        void anslysisData(const SnifferData& snifferData, int row);

    private:
        // Add the frame outline node in the prototree
        QStandardItem* addFrameNode(const SnifferData& snifferData, int row);

        QStandardItemModel* model;
};

#endif
