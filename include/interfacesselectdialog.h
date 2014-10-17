/*
 * =====================================================================================
 *
 *       Filename:  interfacesselectdialog.h
 *
 *    Description:  The interfaces selected dialog
 *
 *        Version:  1.0
 *        Created:  2014年10月12日 00时56分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#ifndef INTERFACESSELECTDIALOG_H_
#define INTERFACESSELECTDIALOG_H_

#include <QDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QCheckBox>
#include <QtCore/QStringList>

#include "../ui/ui_interfacesselectdialog.h"
#include "sniffer.h"

class InterfacesSelectDialog : public QDialog, public Ui::InterfacesSelectDialog
{
    Q_OBJECT

    public:
        InterfacesSelectDialog(Sniffer* pSniffer,QWidget* parent=0);
        ~InterfacesSelectDialog();

    signals:
        // Set capture options
        // Parameters:
        //   bPromiscuous: if set the model promiscuous
        //   deviceIndex: The select device index
        //   strFilter: The filter string
        void setCaptureOptions(const bool bPromiscuous, 
                const int deviceIndex, const QString& strFilter);
            
    private slots:
        void updateBnfExpr(int);
        void checkBnfExpr();
        void startCapture();
        void interfaceSelectChanged(int,int);

    private:
        // Add the dev info to the list widget
        void addNetDevInfo();

        Sniffer* pSniffer;
        int selectedRow; // The selected interface row index
};

#endif
