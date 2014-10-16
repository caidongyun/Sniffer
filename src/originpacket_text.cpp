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

void OriginPacketText::setSelection(int offset ,int len)
{   
    int showByteCnt = this->bHex ? 16 : 8;
    QList<QTextEdit::ExtraSelection> extraSelections;
    qDebug("offset:%d--len:%d", offset,len);

    int start = 0;
    int end = 0;
    for (int i=0; i<len; i+= showByteCnt)
    {
        QTextEdit::ExtraSelection selection;
        selection.cursor = this->textCursor();
        selection.format.setForeground(QColor("#ffffff"));
        selection.format.setBackground(QColor("#4a90d9"));
 
        qDebug("startInd:%d---endInd:%d",offset + i, offset+((i+showByteCnt-1) < len?(i+showByteCnt-1):len));
        start = positionOfByteOffset(offset + i);
        end = positionOfByteOffset(offset + ((i+showByteCnt-1) < len?(i+showByteCnt-1):len));
        //end = positionOfByteOffset(offset + 17);
        selection.cursor.setPosition(start, QTextCursor::MoveAnchor);
        qDebug("i:%d ==start:%d--end:%d",i,start, end);
        
        selection.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,end - start - 1);
        extraSelections.append(selection);

        QTextEdit::ExtraSelection selection2;
        selection2.cursor = this->textCursor();
        selection2.format.setForeground(QColor("#ffffff"));
        selection2.format.setBackground(QColor("#4a90d9"));
 
        start = positionOfCharOffset(offset + i);
        end = positionOfCharOffset(offset + ((i+showByteCnt-1) < len?(i+showByteCnt-1):len));
        //end = positionOfCharOffset(offset + 17);
        selection2.cursor.setPosition(start, QTextCursor::MoveAnchor);
        qDebug("i:%d ==start:%d--end:%d",i,start, end);
        
        selection2.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,end - start - 1);
        extraSelections.append(selection2);

    }
    this->setExtraSelections(extraSelections);
    this->textCursor().clearSelection();
}

int OriginPacketText::positionOfByteOffset(int offset)
{
    int byteRowCnt = 0;   // The total char size in a len
    
    if (this->bHex)
    {
        byteRowCnt = 6 +        // The row number like 0000 and the bigspace 2
            (8 * 2 + 7 * 1)*2 +   // The two hex bit area
            2*2 + (20);         // The char area

    }
    else
    {
        byteRowCnt = 6 + (8*8 + 7*1) + 2 + 10;
    }

    int showByteCnt = this->bHex ? 16 : 8;

    int rowoffset = offset % showByteCnt;
    if (offset%showByteCnt == showByteCnt-1)
    {
        rowoffset += 1;
    }

    int position = offset / showByteCnt * byteRowCnt;
    position += 6;
    if (this->bHex) 
    {
        position += (rowoffset * 2 +                   // The row offset
                ((rowoffset > 1)?(rowoffset-1):0)*1  // the space
                );
        if (rowoffset >= 8)
        {
            position += 2;
        }
    }
    else
    {
        position += (rowoffset * 8 +                   // The row offset
                ((rowoffset > 1)?(rowoffset-1):0)*1  // the space
                );
    }
    if (offset % showByteCnt != 0)
    {
        // If the start is not the start of line add a space char
        position ++;
    }

    return position;
}

int OriginPacketText::positionOfCharOffset(int offset)
{
    int byteRowCnt = 0;   // The total char size in a len
    
    if (this->bHex)
    {
        byteRowCnt = 6 +        // The row number like 0000 and the bigspace 2
            (8 * 2 + 7 * 1)*2 +   // The two hex bit area
            2*2 + (20);         // The char area

    }
    else
    {
        byteRowCnt = 6 + (8*8 + 7*1) + 2 + 10;
    }

    int showByteCnt = this->bHex ? 16 : 8;

    int rowoffset = offset % showByteCnt;
    if (offset%showByteCnt == showByteCnt-1)
    {
        rowoffset += 1;
    }

    int position = offset / showByteCnt * byteRowCnt;
    position += 6;
    if (this->bHex) 
    {
        position += 52;
    }
    else
    {
        position += 74;
    }
    position += (rowoffset + (rowoffset >= 8?1:0));                   // The row offset
    if (offset % showByteCnt != 0)
    {
        // If the start is not the start of line add a space char
        position ++;
    }

    return position;

}
