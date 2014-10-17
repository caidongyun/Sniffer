/*
 * =====================================================================================
 *
 *       Filename:  interfacesselectdialog.cpp
 *
 *    Description:  For the more, see ../include/interfacesselectdialog.h
 *
 *        Version:  1.0
 *        Created:  2014年10月12日 01时08分04秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#include "../include/interfacesselectdialog.h"

InterfacesSelectDialog::InterfacesSelectDialog(Sniffer* pSniffer,QWidget* parent) : QDialog(parent)
{
    this->setupUi(this);
    this->pSniffer = pSniffer;
    this->selectedRow = -1;

    this->startBtn->setEnabled(false);

    // Init the dialog
    this->cancleBtn->setDefault(true);
    this->setMinimumWidth(800);

    this->simpleFilterComboBox->addItems(QStringList() << 
            "" << "Only Ip" <<"Only UDP" << "Only TCP" <<"Only ARP");

    // Add the tablewidget 
    addNetDevInfo();

    QObject::connect(this->cancleBtn, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(this->simpleFilterComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(updateBnfExpr(int)));
    QObject::connect(this->compileBNFBtn, SIGNAL(clicked()),
            this, SLOT(checkBnfExpr()));
    QObject::connect(this->startBtn, SIGNAL(clicked()),
            this,SLOT(startCapture()));
    QObject::connect(this->devSelectTableWidget, SIGNAL(cellChanged(int,int)),
            this, SLOT(interfaceSelectChanged(int,int)));
}

InterfacesSelectDialog::~InterfacesSelectDialog()
{

}

void InterfacesSelectDialog::updateBnfExpr(int index)
{
    QString expr;
    switch(index)
    {
        case 1: // Only Ip
            expr = "ip";
            break; 
        case 2: // Only UDP
            expr = "udp";
            break; 
        case 3: // Only Tcp
            expr = "tcp";
            break;
        case 4: // Only ARP
            expr = "arp";
            break;
        default:
            qDebug("The bnfFileterCombox return invalid index");
            break;
    }

    this->bnfFilterLineedit->setText(expr);
}

void InterfacesSelectDialog::checkBnfExpr()
{
    QString mess = pSniffer->checkBnfExpr(0,this->bnfFilterLineedit->text().simplified());
    if (mess == NULL)
    {
        QMessageBox::information(this, "Info", "The expresion is valid", QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(this, "Error", "The expresion is not valid, Error message is:" + mess, QMessageBox::Ok);
        this->bnfFilterLineedit->setFocus();
    }
}

void InterfacesSelectDialog::addNetDevInfo()
{

    this->devSelectTableWidget->verticalHeader()->setVisible(false);  
    this->devSelectTableWidget->horizontalHeader()->setVisible(true);

    this->devSelectTableWidget->setRowCount(this->pSniffer->netDevsInfoVector.size());
    this->devSelectTableWidget->setColumnCount(3);
    this->devSelectTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->devSelectTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    this->devSelectTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->devSelectTableWidget->setHorizontalHeaderLabels(QStringList() << 
            "Capture" << "Device name" << "Device description");

    this->devSelectTableWidget->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    this->devSelectTableWidget->horizontalHeader()->setClickable(false);
    QTableWidgetItem* item;
    int row = 0;

    foreach (NetDevInfo net, this->pSniffer->netDevsInfoVector)
    {
        item = new QTableWidgetItem();
        item->setCheckState(Qt::Unchecked);
        this->devSelectTableWidget->setItem(row, 0, item);
        item = new QTableWidgetItem(net.strDevName);
        this->devSelectTableWidget->setItem(row, 1, item);

        item = new QTableWidgetItem(net.strDevDesc);
        this->devSelectTableWidget->setItem(row, 2, item);

        row ++;
    }
}


void InterfacesSelectDialog::startCapture()
{
    bool bPromiscuous = this->usePromiscuousRadioBtn->isChecked();
    QString strFilter = this->bnfFilterLineedit->text().simplified();

    emit setCaptureOptions(bPromiscuous, this->selectedRow, strFilter);
    this->hide();
}

void InterfacesSelectDialog::interfaceSelectChanged(int row,int column)
{
    if (this->devSelectTableWidget->item(row,column)->checkState() == Qt::Checked)
    {
        if (this->selectedRow != -1)
        {
            // Only can select one interface, if selected before,
            // unselect it
            this->devSelectTableWidget->item(this->selectedRow,0)->setCheckState(Qt::Unchecked);
        }
        this->selectedRow = row;
    }
    else
    {
        this->selectedRow = -1;
    }

    if (this->selectedRow != -1)
    {
        // Enable the start button
        this->startBtn->setEnabled(true);
    }
    else
    {
        this->startBtn->setEnabled(false);
    }
}
