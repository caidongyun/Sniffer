/*
 * =====================================================================================
 *
 *       Filename:  originpacket_text.cpp
 *
 *    Description:  See ../include/originpacket_text.h
 *
 *        Version:  1.0
 *        Created:  2014年10月09日 22时30分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ping Zeng (zp), zengping@ncic.ac.cn
 *   Organization:  HPC Research Center
 *
 * =====================================================================================
 */
#include "../include/originpacket_text.h"

OriginPacketText::OriginPacketText(QWidget* parent) : QTextEdit(parent)
{
    this->biggerSpace = "  ";
    this->litterSpace = " ";

    QFont font;
    font.setPointSize(10);
    font.setFamily("mono");
    this->setFont(font);
    
}

void OriginPacketText::addData(const QByteArray& rawdata)
{
    QByteArray rd = rawdata;

    int size = rd.size();

    qDebug("data is :\n%s", QString(rawdata.toHex()).toAscii().data());
    qDebug("size is : %d", size);
    QString resStr = "";
    for (int i=0; i<size; i+=16)
    {
       resStr += QString("%1").arg(i, 4,16, QLatin1Char('0')) + this->biggerSpace; 
       // Bit View
       for (int j=0;j<16; j++)
       {
           if (j == 8)
           {
               resStr += this->biggerSpace;
           }

           if (i + j < size)
           {
               resStr += SnifferUtil::byteToHexStr(rd[i+j],16) + this->litterSpace;
               qDebug("byte:%x---char:%s", (uint8_t)rd[i+j], SnifferUtil::byteToHexStr(rd[i+j],16).toStdString().data());
           }
           else
           {
               resStr += "  " + this->litterSpace;
           }
       }

       resStr += this->biggerSpace;
       // Char View
       for (int j=0;j<16 && i+j < size; j++)
       {
           if (j == 8)
           {
               resStr += this->litterSpace;
           }

           resStr += SnifferUtil::byteToChar(rd[i+j]);
           qDebug("byte:%x---char:%s", (uint8_t)rd[i+j], SnifferUtil::byteToChar(rd[i+j]).toAscii().data());
       }
       resStr += "\n";
    }
    qDebug("data is :\n%s --\n%s", QString(rawdata).toAscii().data(), resStr.toStdString().data());

    this->setText(resStr);
}
