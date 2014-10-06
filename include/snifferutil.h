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
#include <netinet/in.h>
#include <arpa/inet.h>

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
};

#endif
