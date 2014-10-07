/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Sniffer的主函数文件
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

#include <QApplication>
#include <QtCore/QTextCodec>

int main(int argc, char *argv[])
{
// 处理中文乱码问题
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
