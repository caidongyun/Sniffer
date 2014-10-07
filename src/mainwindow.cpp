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
    this->pSnifferthread = NULL;
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
}

MainWindow::~MainWindow()
{

}
