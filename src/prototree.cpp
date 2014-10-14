/*
 * =====================================================================================
 *
 *       Filename:  prototree.cpp
 *
 *    Description:  For the more see ../include/prototree.h
 *
 *        Version:  1.0
 *        Created:  2014年10月11日 17时07分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#include "../include/prototree.h"

ProtoTree::ProtoTree(QWidget* parent) : QTreeView(parent)
{
    this->model = new QStandardItemModel(this);
    this->setModel(this->model);

    this->setHeaderHidden(true);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Set style
    this->setStyleSheet(QString("font: mono"));

    this->analysisThread = new AnalysisThread(this->model);
    this->analysisThread->start();
}

ProtoTree::~ProtoTree()
{
    if (this->model)
    {
        this->model->deleteLater();
        this->model = NULL;
    }

    if (this->analysisThread)
    {
        this->analysisThread->deleteLater();
        this->analysisThread = NULL;
    }
}

void ProtoTree::startAnalysis(const SnifferData& snifferData, int row)
{
    this->analysisThread->anslysisData(snifferData, row);
}


