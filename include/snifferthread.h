/*
 * =====================================================================================
 *
 *       Filename:  snifferthread.h
 *
 *    Description:  the thread which sniffer the net packet and process the packet into
 *                  normal data structure.
 *
 *        Version:  1.0
 *        Created:  2014年09月22日 10时16分42秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */

#ifndef SNIFFERTHREAD_H_
#define SNIFFERTHREAD_H_

#include <QtCore/QThread>

#include <time.h>

#include "sniffer.h"
#include "sniffertypes.h"
#include "snifferutil.h"

class SnifferThread : public QThread
{
    Q_OBJECT

    public:
        SnifferThread();
        SnifferThread(Sniffer* pSniffer);
        ~SnifferThread();
        void run();
        void stopSniffer();
        void startSniffer();

    private:
        Sniffer *pSniffer;
        volatile bool bStop;
};
#endif
