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
    this->bHex = true;
    
    QFont font;
    font.setPointSize(10);
    font.setFamily("mono");
    this->setFont(font);

    this->setReadOnly(true);

    this->createContextMenu();
}


void OriginPacketText::createContextMenu()
{
    hexAction = new QAction(tr("Hex View"), this);
    hexAction->setCheckable(true);
    hexAction->setChecked(true);
    QObject::connect(hexAction, SIGNAL(triggered()), this, SLOT(hexView()));

    bitsAction = new QAction(tr("Bits View"), this);
    bitsAction->setCheckable(true);
    QObject::connect(bitsAction, SIGNAL(triggered()), this, SLOT(bitsView()));

    this->addAction(hexAction);
    this->addAction(bitsAction);

    this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void OriginPacketText::hexView()
{
    this->bHex = true;
    this->hexAction->setChecked(true);
    this->bitsAction->setChecked(false);
    this->setDataText();
}

void OriginPacketText::bitsView()
{
    this->bHex = false;
    this->bitsAction->setChecked(true);
    this->hexAction->setChecked(false);
    this->setDataText();
}

void OriginPacketText::setDataText()
{
    int size = this->rawdata.size();

    //qDebug("data is :\n%s", QString(rawdata.toHex()).toAscii().data());
    //qDebug("size is : %d", size);
    QString resStr = "";

    int showByteCnt = this->bHex ? 16 : 8;
    for (int i=0; i<size; i+=showByteCnt)
    {
       resStr += QString("%1").arg(i, 4,16, QLatin1Char('0')) + this->biggerSpace; 
       // Bit View
       for (int j=0;j<showByteCnt; j++)
       {
           if (j == 8 && this->bHex)
           {
               resStr += this->biggerSpace;
           }

           if (i + j < size)
           {
               if (this->bHex)
               {
                   resStr += SnifferUtil::byteToHexStr(this->rawdata[i+j],16) + this->litterSpace;
               }
               else
               {
                   resStr += SnifferUtil::byteToHexStr(this->rawdata[i+j],2) + this->litterSpace;
               }
           }
           else
           {
               if (this->bHex)
               {
                   // The length of hex byte is 2 char 
                   resStr += "  " + this->litterSpace;
               }
               else
               {
                   // The length of bits view is 8 char
                   // so add 8 spaces
                   resStr += "        " + this->litterSpace;
               }
           }
       }

       resStr += this->biggerSpace;
       // Char View
       for (int j=0;j<showByteCnt && i+j < size; j++)
       {
           if (j == 8 && this->bHex)
           {
               resStr += this->litterSpace;
           }

           resStr += SnifferUtil::byteToChar(this->rawdata[i+j]);
           //qDebug("byte:%x---char:%s", (uint8_t)this->rawdata[i+j], SnifferUtil::byteToChar(this->rawdata[i+j]).toAscii().data());
       }
       resStr += "\n";
    }
    //qDebug("data is :\n%s --\n%s", QString(rawdata).toAscii().data(), resStr.toStdString().data());

    this->setText(resStr);

}


void OriginPacketText::addData(const QByteArray& rawdata)
{
    this->rawdata = rawdata;

    this->setDataText();
}
