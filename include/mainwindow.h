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
#include <QTreeWidget>
#include <QTextBlock>
#include <QSplitter>
#include <QTextEdit>

#include "sniffer.h"
#include "snifferthread.h"
#include "sniffertypes.h"
#include "snifferutil.h"
#include "mytableview.h"

class SnifferThread;
class MyTableView;
class QMessageBox;
class QTreeWidget;
class QTableView;
class QTextEdit;
class QSplitter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public: 
    MainWindow();
    ~MainWindow();

private:
    void CreateMainWidget();

private:
    MyTableView *tableview;
    QTreeWidget *treewidget; 
    QTextEdit *byteTextBlock;
    QTextEdit *charTextBlock;
    QSplitter *mainspliter;

    SnifferThread *pSnifferthread;
    Sniffer *pSniffer;
};

#endif
