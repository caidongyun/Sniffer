/*
 * =====================================================================================
 *
 *       Filename:  prototree.h
 *
 *    Description:  The protocal tree
 *
 *        Version:  1.0
 *        Created:  2014年10月11日 17时06分58秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#ifndef PROTOTREE_H_
#define PROTOTREE_H_

#include <QTreeView>
#include <QStandardItemModel>
#include <QtCore/QThread>

#include "sniffertypes.h"
#include "analysisthread.h"

class QTreeView;
class QStandardItemModel;
class QThread;

class AnalysisThread;

class ProtoTree : public QTreeView
{
    Q_OBJECT

    public:
        ProtoTree(QWidget* parent=0);
        ~ProtoTree();

    public slots:
        void startAnalysis(const SnifferData& snifferData, int row);

    private:
        QStandardItemModel* model;
        AnalysisThread* analysisThread; // The thread that analysis the 
};

#endif
