/*
 * =====================================================================================
 *
 *       Filename:  snifferutil.h
 *
 *    Description:  Some util functions
 *
 *        Version:  1.0
 *        Created:  2014年10月07日 01时11分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#ifndef _SNIFFERUTIL_H
#define _SNIFFERUTIL_H

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QtDebug>
#include <QtCore/QDateTime>

#include <netinet/in.h>
#include <arpa/inet.h>

#define LOGFILEMAX 10000

class SnifferUtil
{
    // Change the ipv6 addr to user readable format
    public:
        static QString ipV6ToHost(struct in6_addr addr)
        {
            char buf[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, (void* )&addr,buf, INET6_ADDRSTRLEN);
    
            return QString(buf);
        }

        static QString macToHost(u_int8_t addr[6])
        {
            QByteArray rawData;
            rawData.setRawData((const char*)addr, 6);
            rawData = rawData.toHex().toLower();
            QString res = "";

            int i;
            for (i=0; i<5; i++)
            {
                res = res + rawData[i] + rawData[i+1] + ":";
            }
            res = res + rawData[i] + rawData[i+1];
            return res;
        }

        static void logHandler(QtMsgType type, const char *msg)
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
        
            QTextStream cout(stdout, QIODevice::WriteOnly);
            cout << strLog << endl;
            //QFile outFile("./debug.log");
            //outFile.open(QIODevice::WriteOnly | QIODevice::Append);
         
            //    /**< the max size of log.txt.*/
            //if(outFile.size()/1000>LOGFILEMAX)
            //{
            //    outFile.close();
            //    outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Truncate);
            //    outFile.close();
            //    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
            //}
         
            //QTextStream ts(&outFile);
            //ts << strLog << endl;
        }

};

#endif
