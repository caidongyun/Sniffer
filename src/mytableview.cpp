/*
 * =====================================================================================
 *
 *       Filename:  mytableview.cpp
 *
 *    Description:  custom tableview, see the ../include/mytableview.h for detail
 *
 *        Version:  1.0
 *        Created:  2014年10月07日 15时02分51秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */

#include "../include/mytableview.h"

MyTableView::MyTableView(QWidget* parent) : QTableView(parent)
{
    // Set Table Style

    this->model = new QStandardItemModel();

    this->setModel(this->model);
    this->clearData();
    this->verticalHeader()->setVisible(false);

    // Set single row selection and not editable
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setShowGrid(false);
     
}

MyTableView::~MyTableView()
{
    
}


void MyTableView::addPacketItem(QString strTime, QString strSrc,
                                QString strDst, QString strProtocol,
                                int iLength,QString info)
{
    QStandardItem *item;

    item = new QStandardItem(QString::number(iPos+1,10));
    model->setItem(iPos, 0, item);
    item = new QStandardItem(strTime);
    model->setItem(iPos, 1, item);
    item = new QStandardItem(strSrc);
    model->setItem(iPos, 2, item);
    item = new QStandardItem(strDst);
    model->setItem(iPos, 3, item);
    item = new QStandardItem(strProtocol);
    model->setItem(iPos, 4, item);
    item = new QStandardItem(QString::number(iLength ,10));
    model->setItem(iPos, 5, item);
    item = new QStandardItem(info);
    model->setItem(iPos, 6, item);

    this->iPos ++;
}

void MyTableView::clearData()
{
    this->iPos = 0;
    this->model->clear();
    this->model->setColumnCount(7);
    model->setHeaderData(0,Qt::Horizontal,tr("No."));
    model->setHeaderData(1,Qt::Horizontal,tr("Time"));
    model->setHeaderData(2,Qt::Horizontal,tr("Source"));
    model->setHeaderData(3,Qt::Horizontal,tr("Destination"));
    model->setHeaderData(4,Qt::Horizontal,tr("Protocal"));
    model->setHeaderData(5,Qt::Horizontal,tr("Length"));
    model->setHeaderData(6,Qt::Horizontal,tr("Info"));


    this->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    this->horizontalHeader()->setResizeMode(6,QHeaderView::Stretch);
}

QStandardItemModel* MyTableView::getModel()
{
    return this->model;
}
