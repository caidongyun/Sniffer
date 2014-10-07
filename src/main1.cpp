/*
 * =====================================================================================
 *
 *       Filename:  main1.cpp
 *
 *    Description:  For test
 *
 *        Version:  1.0
 *        Created:  2014年09月21日 21时13分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include "../include/sniffer.h"
#include "../include/snifferutil.h"
#include "../include/snifferthread.h"

int main()
{
   qInstallMsgHandler(SnifferUtil::logHandler);
   Sniffer sniffer;
   sniffer.getAllNetDevs();
   printf("flag:%d\n", sniffer.openNetDev(0));
   //sniffer.captureOnce();
   //while (sniffer.captureOnce() >= 0);
   SnifferThread *pSnifferThread = new SnifferThread(&sniffer); 
   //SnifferThread *pSnifferThread = new SnifferThread();
   pSnifferThread->start();
   pSnifferThread->wait();
   
   return 0;
}
