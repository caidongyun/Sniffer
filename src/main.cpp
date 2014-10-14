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
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QStyle>
#include <QGtkStyle>

int main(int argc, char *argv[])
{
// 处理中文乱码问题
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

    QApplication a(argc, argv);
    a.setStyle(new QGtkStyle());
    //QFile file(":/qss/common.qss");

    //if (file.open(QFile::ReadOnly))
    //{
    //    qDebug("Open success");
    //    QTextStream textStream(&file);
    //    a.setStyleSheet(textStream.readAll());
    //}
    //else
    //{
    //    qDebug("No such Resource file %s", file.fileName().toStdString().data());
    //}

    MainWindow w;
    w.show();

    return a.exec();
}
