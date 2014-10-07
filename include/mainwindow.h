/*
 * =====================================================================================
 *
 *       Filename:  mainwindow.h
 *
 *    Description:  主窗口实现文件头文件
 *
 *        Version:  1.0
 *        Created:  2014年09月21日 20时16分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include <QMessageBox>

#include "sniffer.h"
#include "snifferthread.h"
#include "sniffertypes.h"
#include "snifferutil.h"

class SnifferThread;
class QMessageBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public: 
    MainWindow();
    ~MainWindow();

private:
    SnifferThread *pSnifferthread;
    Sniffer *pSniffer;
};

#endif
