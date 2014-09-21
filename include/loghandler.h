/*
 * =====================================================================================
 *
 *       Filename:  loghandler.h
 *
 *    Description:  The qt log handler
 *
 *        Version:  1.0
 *        Created:  2014年09月21日 21时35分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */

#ifndef LOGHANDLER_H_
#define LOGHANDLER_H_

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QtDebug>
#include <QtCore/QDateTime>

#define LOGFILEMAX 10000

void logHandler(QtMsgType type, const char *msg)
{
    QString strLog;
    switch (type) {
    case QtDebugMsg:
        strLog = QString(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ")+"[Debug] %1").arg(QObject::tr(msg));
        break;
    case QtWarningMsg:
        strLog = QString(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ")+"[Warn] %1").arg(QObject::tr(msg));
    break;
    case QtCriticalMsg:
        strLog = QString(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ")+"[Critical] %1").arg(QObject::tr(msg));
    break;
    case QtFatalMsg:
        strLog = QString(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ")+"[Fatal] %1").arg(QObject::tr(msg));
        abort();
    }
 
    QFile outFile("./debug.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
 
        /**< the max size of log.txt.*/
    if(outFile.size()/1000>LOGFILEMAX)
    {
        outFile.close();
        outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Truncate);
        outFile.close();
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    }
 
    QTextStream ts(&outFile);
    ts << strLog << endl;
}
#endif
