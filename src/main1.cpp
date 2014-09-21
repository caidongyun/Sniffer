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
#include "../include/sniffer.h"
#include "../include/loghandler.h"

int main()
{
   qInstallMsgHandler(logHandler);
   Sniffer sniffer;
   sniffer.getAllNetDevs();
   sniffer.openNetDev(0);
   while (sniffer.captureOnce() >= 0);
   return 0;
}
