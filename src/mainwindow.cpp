/*
 * =====================================================================================
 *
 *       Filename:  mainwindow.cpp
 *
 *    Description:  主窗口实现文件
 *
 *        Version:  1.0
 *        Created:  2014年09月21日 20时14分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#include "../include/mainwindow.h"

MainWindow::MainWindow()
{
    
    CreateMainWidget();

    this->pSniffer = new Sniffer();
    
    if (pSniffer->getAllNetDevs() == false)
    {
        QMessageBox::warning(this, tr("Sniffer"),
                tr("<H3>无法在您的机器上获取可用的网络适配器接口</H3>"
                    "<P>可能有如下原因:</P>"
                    "<ul><li>不支持您的网卡,请到 <a href=\"http://winpcap.org\">"
                    "http://winpcap.org</a> 查阅支持的硬件列表\n</li>"
                    "<li>您的杀毒软件或者防火墙阻止本软件运行</li></ul>"), 
                    QMessageBox::Ok);
    }
    this->pSniffer->openNetDev(0);
    this->pSnifferthread = new SnifferThread(pSniffer, this->tableview);
    connect(this->pSnifferthread,SIGNAL(addPacketItem(QString, QString, QString,QString, int, QString)),
            this->tableview,SLOT(addPacketItem(QString, QString, QString,QString, int, QString)));
    this->pSnifferthread->start();
}

MainWindow::~MainWindow()
{
    if (this->pSniffer)
    {
        delete this->pSniffer;
        this->pSniffer = NULL;
    }
    if (this->pSnifferthread)
    {
        this->pSnifferthread->stopSniffer();
        delete this->pSnifferthread;
        this->pSnifferthread = NULL;
    }
}

void MainWindow::CreateMainWidget()
{
    this->tableview = new MyTableView;
    this->treewidget = new QTreeWidget;
    this->charTextBlock = new QTextEdit;
    this->byteTextBlock = new QTextEdit;

    this->mainspliter = new QSplitter(Qt::Vertical,this);

    this->mainspliter->addWidget(this->tableview);
    this->mainspliter->addWidget(this->treewidget);
    this->charTextBlock->setText("kkfdhkjdk");
    this->mainspliter->addWidget(this->charTextBlock);
    
    this->setCentralWidget(this->mainspliter);
}
