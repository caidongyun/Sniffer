/*
 * =====================================================================================
 *
 *       Filename:  mytableview.h
 *
 *    Description:  Custom table view that display the packet data
 *
 *        Version:  1.0
 *        Created:  2014年10月07日 14时58分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */

#ifndef MYTABLEVIEW_H_
#define MYTABLEVIEW_H_

#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QtCore/qmath.h>

#include "sniffertypes.h"

class QTableView;
class QStandardItemModel;
class QStandardItem;
class QHeaderView;
class QAbstractItemView;

#define ROLE_INDEX Qt::UserRole + 1

class MyTableView : public QTableView
{
    Q_OBJECT

    public:
        MyTableView(QWidget* parent=0);
        ~MyTableView();

        /* *
         * Clear all the data in the tableview
         * */
        void clearData();

        /* *
         * Get the model of the tableview
         * */
        QStandardItemModel* getModel();

        /* *
         * Filter the table
         * */
        void setFilterString(QString strFilter);

        /* *
         * zoom the text size of the tableview
         * */
        void zoomTextSize(int zoomLevel);

    public slots:
        /**
         * Add the sniffer data to the tableview
         *
         * @param Sniffer See the sniffertype.h for detail
         */
        void addPacketItem(QString strTime, QString strSrc,
                QString strDst, QString strProtocol,int iLength,
                QString info);
         
        // Jump to packet
        //void goFirstPacket();
        //void goLastPacket();
        //void goJumpPacket();
        //void goNextPacket();
        //void goPreviousPacket();

    private:
        QStandardItemModel *model;
        QSortFilterProxyModel *proxyModel;
        int iPos; // The postion of current row
        int textSize;
};

#endif
